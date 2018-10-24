/********************************************************************************************************
HOG特征描述符的实现
算法思路:
1)将图片加载入内存，并且利用cvtColor将图像转换为grayImg
2)利用一阶微分算子Sobel函数，分别计算出grayImg图像X方向和Y方向上的一阶微分/梯度图像
3)根据得到的两幅梯度图像(X方向上的梯度图像和Y方向上的梯度图像)，然后利用cartToPolar函数，计算出这
两幅梯度图像所对应的角度矩阵图像angleMat和梯度幅值矩阵图像magnMat
4)将角度矩阵图像angleMat里面的像素强度值归一化为强度范围在[0，9）这9个范围，每一个范围就代表HOG中
的一个bins
5)以角度为为索引，将梯度幅值图像矩阵magnMat按照九个方向的梯度角度拆分为9幅梯度幅值图像矩阵
6)根据这9个角度，每个角度所对应的梯度幅值图像矩阵，并且利用OpenCv中的积分函数integral分别计算出这9
幅图像所对应的积分图像
==============至此，我们9个梯度方向上，分别对应的的9幅梯度幅值积分图已经计算完毕==================
7)计算整幅图像的梯度方向直方图HOG:要计算整幅图像的，需要先计算每个Block的HOG；要计算每个Block的HOG
要先计算每个Cell的HOG
8)计算单个Cell的HOG:由于9个梯度方向上的9张梯度幅值积分图像已经计算出来，所以这一步的计算很简单，只需
要记性加减计算，具体的函数为cacHOGinCell
9)计算单个Block的HOG:将计算出来的4个Cell的HOG组成一个Block的HOG
10)计算整幅图像的HOG:将计算出来的所有的Block的HOG梯度方向直方图的特征向量首尾相接组成一个维度很大的
整幅图像的梯度方向直方图的HOG特征向量，这个特征向量就是整幅图像的梯度方向直方图特征，这个特征
向量也可以被用于SVM的分类
算法难点:
1)积分图像的概念:网上有关积分图像的Blog一大推，但是很多讲的都不准确，最好的办法是看OpenCv的官方文档
关乎积分函数的讲解，可以结合网上的资料看
2)笛卡尔空间坐标和极坐标的转换(关键是理解一些它们之间相互转换的前提条件)
3)L1范数和L2范数:在使用归一化normalize函数时，考虑一些CV_L2到底是向量的L2范数还是矩阵的L2范数，自己
可以推到一下公式
4)关于HOG的论文，没有使用到积分图的概念，其实在HOG中使用积分图像加速了HOG的计算速度，如果使用先计算
梯度，在计算各个区域的梯度方向和梯度幅值的话，这样计算了太大，会导致HOG的性能有所下降
5)还有，这里的每个Cell的大小是20p*20p,每个Block的大小为4个Cell；当然如果用于行人检测的话，也可以使用
其他的3*3或者5*5组合

*****************************************/
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/nonfree/features2d.hpp>
#include <iostream>

// MLP
#include <string>
#include <vector> 
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/ml.hpp>
#include <time.h>

/*  namespace */
using namespace std;
using namespace cv;
using namespace cv::ml;

/*   define*/
#define NBINS 9   //default is 9
#define THETA 180 / NBINS
#define CELLSIZE 8  // default is 20,8 is better
#define BLOCKSIZE 2 //default is 2
#define R (CELLSIZE * (BLOCKSIZE) * 0.5)

const int  TRAIN_IMG_SVM = 8;
const int  TEST_IMG_SVM = 2;
const int  TRAIN_IMG_ANN = 120;
const int  TEST_IMG_ANN = 27;
const int  CLASS_NUM = 2;
const int  EVERY_CLASS_IMG = 4; // jpg = 18,png = 50
const int  FEATURE_DIM = 21060;

Ptr<SVM> svm = SVM::create();
int BLOCKNUM;

/*  function declear  */
std::vector<Mat> CalculateIntegralHOG(Mat& srcMat);
void cacHOGinCell(cv::Mat& HOGCellMat, cv::Rect roi, std::vector<Mat>& integrals);
cv::Mat getHog(cv::Point pt, std::vector<cv::Mat>& integrals);
Mat cacHOGFeature(cv::Mat srcImage, string srcImgPath);
Mat getTrainTestHOGMat(string is_svm, Mat HOGMat, string train_test, int classNum, int start, int end, bool test);
int imgResize(string path);
int SVM_test(Mat testHOGMat);
int SVM_train(Mat trainHOGMat);
int ANN_MLP_train(Mat HOGMat);
int ANN_MLP_test(Mat HOGMat, int objClass);
int getTestResault(Mat predictionMat, float* data);

/*   用于评价的变量  */
int testRight_0 = 0;
int testRight_1 = 0;
int testRight_2 = 0;

/*  slidingWindow 参数  */
#define slide_W 32
#define slide_H 64
#define slide_stride 100

/********************************************************************************************************
函数功能:
计算积分图像
参数说明:
Mat& srcMat-----------------------存储每个cellHOG特征的行特征向量
2)cv::Rect roi--------------------单个cell的矩形位置
3)std::vector<Mat>& integrals-----存储的9幅积分图像，每一幅积分图像代表一个角度范围或者一个bins
*********************************************************************************************************/
// 计算积分图
std::vector<Mat> CalculateIntegralHOG(Mat& srcMat)
{
	//【1】计算一阶微分的梯度图像
	cv::Mat   sobelMatX;
	cv::Mat   sobelMatY;

	cv::Mat   cannyMatX;
	cv::Mat   cannyMatY;

	// 图像预处理
	//// 高斯滤波，越来越模糊
	//Mat gussaBlur;
	//GaussianBlur(srcMat, gussaBlur, Size(3, 3), 0, 0);
	//imshow("滤波后", gussaBlur);
	//waitKey(0);
	//// 拉普拉斯图像增强
	//Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, 0, 5, 0, 0, -1, 0);
	//Mat enhance;
	//filter2D(srcMat, enhance, CV_8UC3, kernel);
	//imshow("图像增强", enhance);
	//waitKey(0);

	//// 直方图均衡化
	//Mat img_hist;
	//equalizeHist(srcMat, srcMat);
	//imshow("直方图均衡化", srcMat);
	//waitKey(0);

	// sobel 边缘提取
	cv::Sobel(srcMat, sobelMatX, CV_32F, 1, 0, 3, 0.3, 45);  //原来的参数是 48
	cv::Sobel(srcMat, sobelMatY, CV_32F, 0, 1, 3, 0.3, 35);   // 40
	/*imshow("sobelX", sobelMatX);
	waitKey(0);
	imshow("sobelY", sobelMatY);
	cvWaitKey(0);*/

	//// canny边缘提取
	//cv::Canny(srcMat, cannyMatX, 60, 200, 3);
	//cv::Canny(srcMat, cannyMatY, 60, 200, 3);
	//imshow("cannyX", cannyMatX);
	//waitKey(0);
	//imshow("cannyY", cannyMatY);
	//cvWaitKey(0);

	std::vector<Mat> bins(NBINS);
	for (int i = 0; i < NBINS; i++)
	{
		bins[i] = Mat::zeros(srcMat.size(), CV_32F);
	}
	cv::Mat   magnMat;
	cv::Mat   angleMat;
	//【2】坐标转换,根据每一个点X方向和Y方向上的梯度，实现笛卡尔坐标和极坐标的转换
	cartToPolar(sobelMatX, sobelMatY, magnMat, angleMat, true);
	//【3】下面这这两行代码起始是做安全处理的，因为在将笛卡尔坐标转换为极坐标之后，角度的范围在[0,360]
	//     下面这两行代码让所有的角度收缩在[0,180]这个返回
	add(angleMat, Scalar(180), angleMat, angleMat<0);                //如果angleMat<0，则加180
	add(angleMat, Scalar(-180), angleMat, angleMat >= 180);          //如果angleMat>=180，则减180
	//【4】下面这行代码将角度矩阵转换为一个灰度值范围在[0,9]之间的图像
	angleMat /= THETA;
	//cout << "--------------" << endl;
	//for (int i = 0; i < magnMat.rows; i++)
	//{
	//	for (int j = 0; j < magnMat.cols; j++)
	//		cout << magnMat.at<float>(i, j)<<' ';
	//	cout << endl;
	//}
	//cout << "---------------------" << endl;
	//for (int i = 0; i < angleMat.rows; i++)
	//{
	//	for (int j = 0; j < angleMat.cols; j++)
	//		cout << angleMat.at<float>(i, j)<<' ';
	//	cout << endl;
	//}
	//【5】下面这个循环，其实是将图像的梯度幅值矩阵按九个不同方向的梯度角度，将每个角度范围内相应点的梯度幅值
	//     存储在相应的矩阵图像之上，其实就是将梯度幅值矩阵图像按照不同的梯度幅值角度分为9幅梯度幅值的图像
	// 9幅梯度幅值图像
	for (int y = 0; y < srcMat.rows; y++)
	{
		for (int x = 0; x < srcMat.cols; x++)
		{
			int ind = angleMat.at<float>(y, x); // ind表示当前像素梯度在第几个bin
			bins[ind].at<float>(y, x) += magnMat.at<float>(y, x);
		}
	}
	//【6】根据上面生成的9张不同角度的梯度幅值矩阵生成9张不同的梯度幅值的积分图像，至此以后，
	//     积分图像的每一点就代表：这一点左上角所有梯度幅值之和；生成的9幅积分图也就是9个
	//     bins，不同bins上的HOG强度
	std::vector<Mat> integrals(NBINS);
	for (int i = 0; i < NBINS; i++)
	{
		integral(bins[i], integrals[i]);
	}
	//积分图是什么？往下看
	return integrals;
}
/********************************************************************************************************
函数功能:
计算单个cell HOG特征
参数说明:
1)cv::Mat& HOGCellMat-------------存储每个cellHOG特征的行特征向量
2)cv::Rect roi--------------------单个cell的矩形位置
3)std::vector<Mat>& integrals-----存储的9幅积分图像，每一幅积分图像代表一个角度范围或者一个bins
*********************************************************************************************************/
void cacHOGinCell(cv::Mat& HOGCellMat, cv::Rect roi, std::vector<Mat>& integrals)
{
	//【1】通过9幅积分图像快速实现HOG的计算，HOG这个直方图有9个bins，每个bins就对应一张积分图像
	int x0 = roi.x;                              //确定单个矩形cell的左上角点坐标
	int y0 = roi.y;
	int x1 = x0 + roi.width;
	int y1 = y0 + roi.height;                    //确定单个矩形cell的右下角点坐标

	for (int i = 0; i <NBINS; i++)
	{
		//【2】根据矩形的左上角点和右下角点的坐标
		cv::Mat integral = integrals[i];

		float a = integral.at<double>(y0, x0);
		float b = integral.at<double>(y1, x1);
		float c = integral.at<double>(y0, x1);
		float d = integral.at<double>(y1, x0);

		HOGCellMat.at<float>(0, i) = b - c - d + a;//每循环一次，计算一个梯度方向上的HOG特征，其实就是
		//每循环一次，就计算梯度方向直方图上的一个bins
	}
}
/****
获取当前窗口的HOG直方图----此块其实就是在计算单个Block的HOG梯度方向直方图
1)cv::Point pt--------------------单个Block的中心点坐标
2)std::vector<cv::Mat>& integrals-----存储的9幅积分图像，每一幅积分图像代表一个角度范围或者一个bins
**********/
cv::Mat getHog(cv::Point pt, std::vector<cv::Mat>& integrals)
{
	if (pt.x - R<0 || pt.y - R<0 || pt.x + R >= integrals[0].cols || pt.y + R >= integrals[0].rows)
	{
		return cv::Mat();
	}
	//【1】BLOCK的HOG直方图---具体的来说，BLOCKSIZE*BLOCKSIZE即4个cell的HOG特征直方图特征向量
	//     组成一个BLOCK的HOG特征直方图的特征向量
	cv::Mat    hist(cv::Size(NBINS*BLOCKSIZE*BLOCKSIZE, 1), CV_32F);
	cv::Point  t1(0, pt.y - R);
	int c = 0;
	//【2】遍历块:通过下面这两个循环，就遍历了4个cell，并且将4个cell的HOG特征向量组成了一个
	//     维数比较大的BLOCK的HOG特征向量
	for (int i = 0; i<BLOCKSIZE; i++)
	{
		t1.x = pt.x - R;
		for (int j = 0; j<BLOCKSIZE; j++)
		{
			//【3】获取当前窗口，进行局部HOG直方图计算
			cv::Rect roi(t1, t1 + cv::Point(CELLSIZE, CELLSIZE));
			cv::Mat  hist_temp = hist.colRange(c, c + NBINS);
			//【4】根据roi确定的矩形区域，计算单个cell的HOG直方图(其本质就是一个行特征向量)
			cacHOGinCell(hist_temp, roi, integrals);
			t1.x += CELLSIZE;
			c += NBINS;
		}
		t1.y = CELLSIZE;
	}//for i
	//【3】利用范数2进行归一化
	cv::normalize(hist, hist, 1, 0, NORM_L2);
	return hist;
}

//计算整幅图像的HOG梯度方向直方图---HOG特征，并把整幅图HOG特征描述放入指定矩阵中
Mat cacHOGFeature(cv::Mat srcImage, string srcImgPath)
{
	cv::Mat          grayImage;
	
	cv::cvtColor(srcImage, grayImage, CV_RGB2GRAY);
	grayImage.convertTo(grayImage, CV_8UC1);// 变成单通道的灰度图
	
	 std::vector<Mat> HOGFeatureVector;
	// 如果定义全局的HOGMatVector，便于在svmtrain中获取其大小，但需要
	// HOGMatVector.clear();
	 cout << " 函数内：" << HOGFeatureVector.size() << endl;
	
	//【1】9个不同梯度方向上的9张梯度幅值的积分图像的生成
	std::vector<Mat> integrals = CalculateIntegralHOG(grayImage);
	Mat image = grayImage.clone();   //在灰度图上画出HOG特征可视化
	//Mat image(srcImage.rows, srcImage.cols, CV_8UC4);
	image *= 0;
	//【2】遍历全图像，计算最终的梯度方向直方图HOG
	cv::Mat HOGBlockMat(Size(NBINS, 1), CV_32F);
	for (int y = CELLSIZE / 2; y < grayImage.rows; y += CELLSIZE)
	{
		for (int x = CELLSIZE / 2; x < grayImage.cols; x += CELLSIZE)
		{
			//【3】获取当前窗口HOG，其实当前的窗口就是一个Block，每个Block由4个cell组成，每个Cell为20*20
			//     此块，计算的就是单个Block的梯度方向直方图HOG
			cv::Mat hist = getHog(Point(x, y), integrals);
			if (hist.empty())
				continue;
			HOGBlockMat = Scalar(0);
			for (int i = 0; i < NBINS; i++)
			{
				for (int j = 0; j < BLOCKSIZE; j++)
				{
					HOGBlockMat.at<float>(0, i) += hist.at<float>(0, i + j*NBINS);
				}
			}
			//【4】L2范数归一化:对其得到的每个Block的的矩阵进行L2范数归一化，使其转变为一个Block的HOG特征向量
			normalize(HOGBlockMat, HOGBlockMat, 1, 0, CV_L2);
			//【5】最后，每得到一个Block的HOG特征向量就存入HOGMatVector，这个HOGMatVector其实就是整个图像的HOG特征向量，
			//     当然，现在这个HOGMatVector还是个二维数组的形式，如果想要利用SVM对其进行分类的话，还需要将其拉伸为一
			//     维特征向量
			HOGFeatureVector.push_back(HOGBlockMat);
			Point center(x, y);
			//【6】绘制HOG特征图
			for (int i = 0; i < NBINS; i++)
			{
				double theta = (i * THETA) * CV_PI / 180.0;
				Point rd(CELLSIZE*0.5*cos(theta), CELLSIZE*0.5*sin(theta));
				Point rp = center - rd;
				Point lp = center + rd;
				//line(image, rp, lp, Scalar(255 * HOGBlockMat.at<float>(0, i), 255, 255));
				line(image, rp, lp, Scalar(255* HOGBlockMat.at<float>(0, i), 0, 0));
			}
		}
	}
	cout << "在原灰度图上显示HOG特征图：" << endl;
	imshow("HOG特征可视化：", image);
	waitKey(0);
	BLOCKNUM = HOGFeatureVector.size();
	cout << "HOG特征维度即blockNumbe为：" << HOGFeatureVector.size() << endl;	
	cout <<"BLOCKNUM*NBINS is "<< BLOCKNUM*NBINS << endl;
	cout << "所设置的特征维度FEATURE_DIM大小为：" << FEATURE_DIM << endl;
	// HOGFeatureVector转换成HOGFeatureMat存储，拉直
	Mat HOGFeatureMat(1, NBINS * BLOCKNUM, CV_32FC1);
	for (int m = 0; m < BLOCKNUM; m++)
	{
		for (int n = 0; n < NBINS; n++)
		{
			int index = m * NBINS + n;
			HOGFeatureMat.at<float>(0, index) = HOGFeatureVector[m].at<float>(0, n);
			//cout << HOGFeatureMat.at<float>(0, index) << endl;
		}
	}
	return HOGFeatureMat; //返回每张图像的整个特征矩阵
}

// MLP function

//得到用于训练或者测试的特征矩阵
Mat getTrainTestHOGMat(string is_svm,Mat HOGMat, string train_test, int classNum, int start, int end, bool test )
{
	int smpW = FEATURE_DIM;// NBINS * BLOCKNUM;
	int smpH = 1;
	// 准备待训练的HOG特征描述矩阵
	//Mat HOGMat(TRAIN_IMG_ALL, smpW * smpH, CV_32FC1);
	for (int i = 0; i < classNum; i++)
	{
		for (int j = start; j < end; j++)
		{
			string path = format("images\\srcImg\\tower\\%s\\%d%d.jpg", train_test.c_str(), i, j);
			string name = format("tower\\%s\\%d%d.jpg", train_test.c_str(), i, j);
			cout << name << endl;
			// 将图像resize到指定大小
			imgResize(path);
			
			Mat image = imread(path);
			if (image.empty())
			{
				cout << "没有读取到图像" << endl;
				return Mat();
			}
			namedWindow(name, WINDOW_KEEPRATIO);
			imshow(name, image);
			waitKey(0);
			Mat HOGFeatureMat = cacHOGFeature(image, name);
			cout << "获取到了HOGMatVector,vector大小为：" << BLOCKNUM << endl;
			int index;
			if (train_test == "train")
			{
				index = i * (end - start) + j - start;  //index计算有错误，后面应该再加上i
				float* Data = HOGMat.ptr<float>(index);
				cout << "将提取第" << index + 1 << "张图像的HOG特征，并将其存入待训练矩阵中" << endl;
			}
			if (train_test == "test")
			{
				index = 0;
				float* Data = HOGMat.ptr<float>(index);
				cout << "识别输入的图像的特征" << endl;
			}
			cout << " HOGMat.rows =  "<<HOGMat.rows <<"\t"<< "HOGMat.cols = "<< HOGMat.cols << endl;
		//	cout << "---------------------------" << endl;
			for (int n = 0; n < FEATURE_DIM; n++)
			{
				HOGMat.at<float>(index, n) = HOGFeatureMat.at<float>(0, n);
			}
			if (test)
			{
				if (is_svm == "SVM")
					 SVM_test(HOGMat);
				if(is_svm == "ANN") 
					ANN_MLP_test(HOGMat,i);
			}
		}
	}
	return HOGMat;
}

int SVM_train(Mat trainHOGMat)
{
	//训练 CLASS_NUM 类数据，每类 EVERY_CLASS_NUM 张
	int labels[TRAIN_IMG_SVM] = { 0, 0, 0, 0, 1, 1, 1, 1 };
	Mat labelsMat(TRAIN_IMG_SVM, 1, CV_32SC1, labels);  //将label变成矩阵的形式，数据类型要为CV_32SC1
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 1000, 1e-6)); //训练终止条件

	//设置训练数据
	Ptr<TrainData> train_data = TrainData::create(trainHOGMat, ROW_SAMPLE, labelsMat);
	cout << "开始训练" << endl;
	clock_t a, b;
	a = clock();
	svm->train(train_data);
	b = clock();
	cout << "训练结束，保存模型为hogSvm.xml." << endl;
	cout << "训练所用时间为:" << (double)(b - a) / CLOCKS_PER_SEC <<"seconds"<< endl;
	// 训练用时间为 0.534 秒
	svm->save("hogSvm.xml");

	return 0;
}

int SVM_test(Mat testHOGMat)
{
	cout << "加载SVM模型hogSvm.xml." << endl;
	svm = SVM::load<SVM>("hogSvm.xml");

	//存放预测结果的矩阵
	Mat predictionMat(1, 1, CV_32S);
	float response = svm->predict(testHOGMat, predictionMat);
	float* data = predictionMat.ptr<float>(0);
	//二分类
	cout << "预测结果为" << data[0] << endl;
	cout << "svm test" << endl;
	return data[0];
}

int imgResize(string path)
{
	for (int i = 0; i < CLASS_NUM; i++)
	{
		for (int j = 1; j < 7; j++)
		{
			Mat img = imread(path, 0);
			if (img.empty())
			{
				return 0;
			}
			else
			{
				// 图像大小，与 FEATURESIZE 相互关联
				resize(img, img, Size(256, 640));
				imwrite(path, img);
			}
		}
	}
	return 0;
}

int ANN_MLP_train(Mat trainMat)
{
	float labels[120][3] = {
		{ 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 },
		{ 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 },
		{ 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 },
		{ 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 }, { 1, 0, 0 },

		{ 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 },
		{ 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 },
		{ 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 },
		{ 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 },

		{ 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 },
		{ 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 },
		{ 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 },
		{ 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }
	};  // 每个样本数据对应的输出	
	// TODO 
	// 改为从文件中读取label。
	Mat labelsMat(120, 3, CV_32FC1, labels);  // 原始为8，1，行数与 列数与MLP输出相同
	cout << "标签矩阵为：" << endl;
	cout << labelsMat << endl;

	cout << "构造MLP分类器" << endl;
	Ptr<ANN_MLP> bp = ANN_MLP::create();
	Mat layerSizes = (Mat_<int>(1, 3) << FEATURE_DIM, 6, 3);
	bp->setLayerSizes(layerSizes);
	bp->setTrainMethod(ANN_MLP::BACKPROP, 0.1, 0.1);
	bp->setActivationFunction(ANN_MLP::SIGMOID_SYM);
	bp->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 1000, 1e-6));
	cout << "开始训练啦...去喝杯咖啡吧放松下吧..." << endl;
	clock_t a, b;
	a = clock();
	bool trained = bp->train(trainMat, ROW_SAMPLE, labelsMat);
	b = clock();
	
	if (trained)
	{
		bp->save("ANN_MLP.xml");
		cout << "保存训练好的模型为ANN_MLP.xml" << endl;
		cout << "训练所用的时间为：" << (double)(b - a) / CLOCKS_PER_SEC << " seconds " << endl;
		// 训练时间为 1565.27 秒，约为27分钟
	}
	return 0;
}

// 用于实验测试准确率
int ANN_MLP_test(Mat testMat,int objClass)
{
	cout << "*******************************"<<endl;
	Ptr<ANN_MLP> bp = ANN_MLP::create();
	cout << "加载预模型文件 ANN_MLP.xml ..." << endl;
	bp = ANN_MLP::load<ANN_MLP>("ANN_MLP.xml");   //加载训练好的模型
	/*cout << bp.empty() << endl;
	if (! bp.empty())
	{
		cout << "加载预训练模型失败，请先训练" << endl;
		return -1;
	}*/
	cout << "加载完成" << endl;
	Mat predictionMat(1, 3, CV_32FC1);
	float response = bp->predict(testMat, predictionMat);  // 分类问题，则误差表示识别率
	float* data = predictionMat.ptr<float>(0); //data[0]取出第一行第一个数
	cout << "预测矩阵：" << predictionMat << endl;	
	//根据输出矩阵中最大的一个，判断是哪个类别
	//应该加入一个阈值，0.4，低于阈值时候未检测到任何目标
	float max = data[0];
	int tmp = 0;
	for (int i = 0; i < predictionMat.cols; i++)
	{
		if (data[i]>max)
		{
			max = data[i];
			tmp = i;
		}
	}
	if (max > 0.2 )
	{
		switch (tmp)
		{
			case 0: cout << "预测结果为：" << "第0类," << "概率为" << max  << endl;  break;
			case 1: cout << "预测结果为：" << "第1类," << "概率为" << max  << endl;  break;
			case 2: cout << "预测结果为：" << "第2类," << "概率为" << max  << endl;  break;
			default: break;
		}
	}
	else
		cout << "error 未检测到任何类别！" << endl;

	if (tmp == objClass)
	{
		switch (tmp)
		{
		case 0: cout << "是识别到的第" << ++testRight_0 << "个" << endl;  break;
		case 1: cout << "是识别到的第" << ++testRight_1 << "个" << endl;  break;
		case 2: cout << "是识别到的第" << ++testRight_2 << "个" << endl;  break;
		default: break;
		}
	}
	else if (objClass == -1)
	{
		cout << "检测摄像头传来的单帧图像" << endl;
	}
	else	
		cout << "识别错误，将第" << objClass << "类识别成了第" << tmp << "类" << endl;
	cout << "*****************************" << endl;
	//waitKey(0);
	int classNumResault = tmp;
	//int classNumResault = getTestResault(predictionMat, data);
	return classNumResault;
}

// 将获取结果的部分代码封装成函数
//int getTestResault(Mat predictionMat,float* data)
//{
//	float max = data[0];
//	int tmp = 0;
//	for (int i = 0; i < predictionMat.cols; i++)
//	{
//		if (data[i]>max)
//		{
//			max = data[i];
//			cout << max;
//			tmp = i;
//		}
//	}
//	if (max > 0.2)
//	{
//		switch (tmp)
//		{
//		case 0: cout << "预测结果为：" << "第0类," << "概率为" << max << endl;  break;
//		case 1: cout << "预测结果为：" << "第1类," << "概率为" << max << endl;  break;
//		case 2: cout << "预测结果为：" << "第2类," << "概率为" << max << endl;  break;
//		default: break;
//		}
//	}
//	else
//		cout << "error 未检测到任何类别！" << endl;
//	cout << "*****************************" << endl;
//	//waitKey(0);
//	return tmp;
//}

int main()
{
///*【1】数据准备*/
//	//设置HOG特征描述符的宽高
	cout << "准备数据中..." << endl;
	// 准备待训练的HOG特征描述矩阵
	Mat trainHOGMat(TRAIN_IMG_SVM, FEATURE_DIM, CV_32FC1);
	Mat testHOGMat(1, FEATURE_DIM, CV_32FC1);

/*【2】读原始图像，提取HOG特征描述*/
	trainHOGMat = getTrainTestHOGMat("SVM",trainHOGMat, "train", CLASS_NUM, 1, 5,false);
	cout << "数据准备完毕，准备训练SVM..." << endl;

/*【3】 训练SVM、MLP模型*/
	/* SVM */
	SVM_train(trainHOGMat);
	/*  ANN  */
	Mat trainHOGMatANN(TRAIN_IMG_ANN, FEATURE_DIM, CV_32FC1);
	Mat testHOGMatANN(1, FEATURE_DIM, CV_32FC1);
	trainHOGMatANN = getTrainTestHOGMat("ANN",trainHOGMatANN, "train", 2, 1, 41,false);
	cout << "ANN 数据准备完毕，开始训练啦..." << endl;
	ANN_MLP_train(trainHOGMatANN);

/*【4】 模型测试*/
	testHOGMat = getTrainTestHOGMat("SVM", testHOGMat, "test", CLASS_NUM, 5, 6, true);
	SVM_test(testHOGMat);  //每获得一张图像就需要检测
	testHOGMatANN = getTrainTestHOGMat("ANN", testHOGMatANN, "test", 3, 41, 50, true); //27张图片，感觉testHOGMatANN做返回值也没什么用



/*  注意 
0、先单类实验
1、注意训练SVM时候截取原图像大小部分区域训练，截取 size_W * size_H 大小窗口进行训练
2、训练好的模型，去检测。这时候注意，训练用的 FEATURE_DIM 使用新的数据的 维度
3、实际检测的时候，从原图也是获取
这么大小的ROI区域进行检测分类

4、注意，姿态检测的时候，可以直接拿整张图像进行姿态检测

*/



/*【5】 滑动窗口循环获取目标区域，然后提取HOG，送入到分类器中检测*/
	/*  
	  1、获取摄像头图像
	  2、设置滑窗，提取滑窗区域的图像ROI
	  3、提取ROI区域的HOG特征向量矩阵
	  4、将ROI的HOG特征矩阵送入SVM分类器模型进行检测，背景-目标二分类
	  5、根据分类结果 
				a）如果是目标，就记录当前的框的左顶点位置，放入容器中
				b）如果不是目标，则接着找下一个slidingWindow区域
	  6、重复执行2-5，指导遍历完图像
	  7、对记录的pointRemembered的的point.x  point.y分别进行排序
		 找到point.x最小的和最大的，找到point.y最小和最大的
		 注意设置阈值，剔除误检测
	  8、思考：深度学习中boundingbox是怎么回归的？回归的方法是怎么实现的？
	*/ 
	// 载入图片
	string path = format("images//srcImg//tower//test//801.jpg");
	Mat imgFromCam = imread(path, CV_LOAD_IMAGE_COLOR);
	if ( imgFromCam.empty() )
		return -1;
	// 提取ROI
	Mat lineSrcImg = imgFromCam.clone();  // image.clone() 图像拷贝    
	int img_H = imgFromCam.rows;
	int img_W = imgFromCam.cols;
	int x = 0, y = 0;
	int windowCount = 0;
	vector<int> pointRemX;  //存放识别结果为目标的各个点的X坐标和Y坐标
	vector<int> pointRemY;

	Point pointA, pointB, pointC, pointD; //boundingbox 的四个坐标点
	for (y = 0; y < img_H - slide_H; y = y + slide_H/2)
	{
		for (x = 0; x < img_W - slide_W; x = x + slide_W/2)
		{
			Rect slidingWindow(x, y, slide_W, slide_H);
			cout << "滑窗的面积为：" << slidingWindow.area() << endl;
			cout << "各个滑窗顶点坐标为:" << "(" << x << "," << y << ")" << endl;
			Mat ROI = imgFromCam(Rect(x, y, slide_W, slide_H));
			++windowCount;
			string newName = format("slideWindowImgs\\%d.jpg", windowCount);
			namedWindow("ROI", WINDOW_FREERATIO);
			imshow("ROI", ROI);
			imwrite(newName, ROI);

			// 画框 boundingbox
		  //	Mat lineSrcImg = imgFromCam.clone();
			pointA.x = x;  // 左上
			pointA.y = y;
			pointD.x = x + slide_W;  // 右下
			pointD.y = y + slide_H;
			pointB.x = x + slide_W;
			pointB.y = y;
			pointC.x = x;
			pointC.y = y + slide_H;

			//circle(lineSrcImg, pointA, 4, Scalar(0, 0, 0), -1); // thickness = -1 表示画实心圆
			//circle(lineSrcImg, pointB, 4, Scalar(0, 0, 0), -1);
			
			line(lineSrcImg, pointA, pointB, Scalar(0, 0, 255), 1);
			imshow("lineSrcImg", lineSrcImg);
			cvWaitKey(0);

			//circle(lineSrcImg, pointC, 4, Scalar(0, 0, 0), -1);
			line(lineSrcImg, pointA, pointC, Scalar(0, 0, 255), 1);
			
			imshow("lineSrcImg", lineSrcImg);
			cvWaitKey(0);
		
			//circle(lineSrcImg, pointD, 4, Scalar(0, 0, 0), -1);
			line(lineSrcImg, pointC, pointD, Scalar(0, 0, 255), 1);
			imshow("lineSrcImg", lineSrcImg);
			cvWaitKey(0);

			line(lineSrcImg, pointD, pointB, Scalar(0, 0, 255), 1);
			imshow("lineSrcImg", lineSrcImg);
			cvWaitKey(0);

			//// 画斜线
			//line(srcImg, pointA, pointD, Scalar(0, 0, 255),1);
			//imshow("lineSrcImg", srcImg);

			// 检测ROI区域
			// 检测到ROI区域是目标之后，记录位置，存入vector中
			// 这部分需要重新做下，训练SVM的图像和检测用的ROI区域图像的宽高要一致，或者将ROI resize
			// TODO here
 		//	int prediction =	SVM_test(ROI); // 这里的ROI的维度不对，应该用ROI大小的区域重新训练SVM分类器
			//if (prediction = 0)
			//{
			//	cout << "检测出来的是背景，检测下一个ROI" << endl;
			//	break;
			//}
			//else
			//{
			//	pointRemX.push_back(pointA.x);
			//	pointRemY.push_back(pointA.y);
			//}
		}
	}
	/* 整张图像循环结束，滑窗完成<考虑使用2个滑窗，使用多线程实现>
	*  TODO：对vector中的数据进行排序，获取其中x 最大最小值，y最大最小值
	*  该矩形区域就是最终识别并定位的目标，同时送入到MLP分类器中分类杆塔类型
	*  或者，使用DMP部件模型识别目标姿态  
	*/

	// 对框的左上点x,y值进行排序，获取最小的x,y，与最大的x,y
	// sort中的排序算法是怎么实现的？
	sort(pointRemX.begin(), pointRemX.end());  // sort排序，排序好的数据在原来的容器中，默认是
	sort(pointRemY.begin(), pointRemY.end());
	Point minPoint;
	Point maxPoint;
	minPoint.x = pointRemX[pointRemX.size() - 1];
	minPoint.y = pointRemY[pointRemY.size() - 1];
	maxPoint.x = pointRemX[0];
	maxPoint.y = pointRemY[0];

	// 以排序后的最小的x,y作为allROIs的左上顶点；以排序后的最大的x,y再加上box的宽高作为allROIs的右下顶点
	Mat allROIs = imgFromCam(Rect(minPoint.x, minPoint.y,maxPoint.x+slide_W,maxPoint.y+slide_H));

	// TODO
	// 将allROIs交给MLP分类器分类不同的杆塔，MLP分类器训练样本设置为指定合适大小，比如 300*300？
	// 然后将ROI区域resize一下，再用于检测
	resize(allROIs, allROIs, Size(300, 300));
	ANN_MLP_test(allROIs,-1);  // -1 表示用来单独调用ANN_MLP_test()，而非统计识别准确率
	
	// 将allROIs交给姿态分类器分类姿态
	// TODO 
	// DMP?模型？
	// 边写论文边研究这个

	cout << "程序结束咯！" << endl;
	waitKey();
	return 0;
}

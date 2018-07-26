/********************************************************************************************************
文件说明:
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
开发环境:
Win7 + OpenCv2.4.8 + VS2012
时间地点:
陕西师范大学 2017.3.14
作    者:
九 月
*********************************************************************************************************/
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/nonfree/features2d.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#define NBINS 9   //default is 9
#define THETA 180 / NBINS
#define CELLSIZE 8  // default is 20,8 is better
#define BLOCKSIZE 2 //default is 2
#define R (CELLSIZE * (BLOCKSIZE) * 0.5)

#define  classNum  3
#define  imgNum  50  // jpg = 18,png = 50

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
	
	cv::Sobel(srcMat, sobelMatX, CV_32F, 1, 0);
	cv::Sobel(srcMat, sobelMatY, CV_32F, 0, 1);
	//for (int i = 0; i < sobelMatX.rows; i++)
	//{
	//	for (int j = 0; j < sobelMatX.cols; j++)
	//		cout << sobelMatX.at<float>(i, j)<<' ';
	//	cout << endl;
	//}
	//cout << "---------------------" << endl;
	//for (int i = 0; i < sobelMatY.rows; i++)
	//{
	//	for (int j = 0; j < sobelMatY.cols; j++)
	//		cout << sobelMatY.at<float>(i, j)<<' ';
	//	cout << endl;
	//}

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
			int ind = angleMat.at<float>(y, x); // ind表示当前像素梯度在第几个bin，
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

//计算整幅图像的HOG梯度方向直方图---HOG特征
std::vector<Mat> cacHOGFeature(cv::Mat srcImage)
{
	cv::Mat          grayImage;
	std::vector<Mat> HOGMatVector;
	cv::cvtColor(srcImage, grayImage, CV_RGB2GRAY);
	grayImage.convertTo(grayImage, CV_8UC1);// 变成单通道的灰度图

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
			HOGMatVector.push_back(HOGBlockMat);
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
	imshow("out", image);
	return HOGMatVector;
}

int main()
{
	// HOG特征描述暂存

	// 读数据集
	for (int i = 0; i < classNum; i++)
	{
		for (int j = 1; j < imgNum; j++)
		{
			//string srcImgPath = format("images\\srcImg\\charData\\%d%d.png", i, j);
			string srcImgPath = format("images\\srcImg\\tower\\%d%d.jpg", i, j);
			Mat srcImage = imread(srcImgPath);
			if (srcImage.empty())
				break;
			imshow("srcImg", srcImage);
			// 提取HOG特征
			std::vector<Mat> HOGFeatureMat = cacHOGFeature(srcImage);
			cv::waitKey(0);
		}
	}
	return 0;
}

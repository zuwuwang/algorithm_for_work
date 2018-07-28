/********************************************************************************************************
�ļ�˵��:
HOG������������ʵ��
�㷨˼·:
1)��ͼƬ�������ڴ棬��������cvtColor��ͼ��ת��ΪgrayImg
2)����һ��΢������Sobel�������ֱ�����grayImgͼ��X�����Y�����ϵ�һ��΢��/�ݶ�ͼ��
3)���ݵõ��������ݶ�ͼ��(X�����ϵ��ݶ�ͼ���Y�����ϵ��ݶ�ͼ��)��Ȼ������cartToPolar�������������
�����ݶ�ͼ������Ӧ�ĽǶȾ���ͼ��angleMat���ݶȷ�ֵ����ͼ��magnMat
4)���ǶȾ���ͼ��angleMat���������ǿ��ֵ��һ��Ϊǿ�ȷ�Χ��[0��9����9����Χ��ÿһ����Χ�ʹ���HOG��
��һ��bins
5)�ԽǶ�ΪΪ���������ݶȷ�ֵͼ�����magnMat���վŸ�������ݶȽǶȲ��Ϊ9���ݶȷ�ֵͼ�����
6)������9���Ƕȣ�ÿ���Ƕ�����Ӧ���ݶȷ�ֵͼ����󣬲�������OpenCv�еĻ��ֺ���integral�ֱ�������9
��ͼ������Ӧ�Ļ���ͼ��
==============���ˣ�����9���ݶȷ����ϣ��ֱ��Ӧ�ĵ�9���ݶȷ�ֵ����ͼ�Ѿ��������==================
7)��������ͼ����ݶȷ���ֱ��ͼHOG:Ҫ��������ͼ��ģ���Ҫ�ȼ���ÿ��Block��HOG��Ҫ����ÿ��Block��HOG
Ҫ�ȼ���ÿ��Cell��HOG
8)���㵥��Cell��HOG:����9���ݶȷ����ϵ�9���ݶȷ�ֵ����ͼ���Ѿ����������������һ���ļ���ܼ򵥣�ֻ��
Ҫ���ԼӼ����㣬����ĺ���ΪcacHOGinCell
9)���㵥��Block��HOG:�����������4��Cell��HOG���һ��Block��HOG
10)��������ͼ���HOG:��������������е�Block��HOG�ݶȷ���ֱ��ͼ������������β������һ��ά�Ⱥܴ��
����ͼ����ݶȷ���ֱ��ͼ��HOG�����������������������������ͼ����ݶȷ���ֱ��ͼ�������������
����Ҳ���Ա�����SVM�ķ���
�㷨�ѵ�:
1)����ͼ��ĸ���:�����йػ���ͼ���Blogһ���ƣ����Ǻིܶ�Ķ���׼ȷ����õİ취�ǿ�OpenCv�Ĺٷ��ĵ�
�غ����ֺ����Ľ��⣬���Խ�����ϵ����Ͽ�
2)�ѿ����ռ�����ͼ������ת��(�ؼ������һЩ����֮���໥ת����ǰ������)
3)L1������L2����:��ʹ�ù�һ��normalize����ʱ������һЩCV_L2������������L2�������Ǿ����L2�������Լ�
�����Ƶ�һ�¹�ʽ
4)����HOG�����ģ�û��ʹ�õ�����ͼ�ĸ����ʵ��HOG��ʹ�û���ͼ�������HOG�ļ����ٶȣ����ʹ���ȼ���
�ݶȣ��ڼ������������ݶȷ�����ݶȷ�ֵ�Ļ�������������̫�󣬻ᵼ��HOG�����������½�
5)���У������ÿ��Cell�Ĵ�С��20p*20p,ÿ��Block�Ĵ�СΪ4��Cell����Ȼ����������˼��Ļ���Ҳ����ʹ��
������3*3����5*5���
��������:
Win7 + OpenCv2.4.8 + VS2012
ʱ��ص�:
����ʦ����ѧ 2017.3.14
��    ��:
�� ��
*********************************************************************************************************/
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

using namespace std;
using namespace cv;
using namespace cv::ml;

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

std::vector<Mat> CalculateIntegralHOG(Mat& srcMat);
void cacHOGinCell(cv::Mat& HOGCellMat, cv::Rect roi, std::vector<Mat>& integrals);
cv::Mat getHog(cv::Point pt, std::vector<cv::Mat>& integrals);
Mat cacHOGFeature(cv::Mat srcImage, string srcImgPath);
Mat getTrainTestHOGMat(Mat HOGMat, string train_test, int classNum, int start, int end,bool test);
int imgResize(string path);
int SVM_test(Mat testHOGMat);
int SVM_train(Mat trainHOGMat);
int ANN_MLP_train(Mat HOGMat);
int ANN_MLP_test(Mat HOGMat);
int getTestResault(Mat predictionMat, float* data);
/********************************************************************************************************
��������:
�������ͼ��
����˵��:
Mat& srcMat-----------------------�洢ÿ��cellHOG����������������
2)cv::Rect roi--------------------����cell�ľ���λ��
3)std::vector<Mat>& integrals-----�洢��9������ͼ��ÿһ������ͼ�����һ���Ƕȷ�Χ����һ��bins
*********************************************************************************************************/
// �������ͼ
std::vector<Mat> CalculateIntegralHOG(Mat& srcMat)
{
	//��1������һ��΢�ֵ��ݶ�ͼ��
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
	//��2������ת��,����ÿһ����X�����Y�����ϵ��ݶȣ�ʵ�ֵѿ�������ͼ������ת��
	cartToPolar(sobelMatX, sobelMatY, magnMat, angleMat, true);
	//��3�������������д�����ʼ������ȫ����ģ���Ϊ�ڽ��ѿ�������ת��Ϊ������֮�󣬽Ƕȵķ�Χ��[0,360]
	//     ���������д��������еĽǶ�������[0,180]�������
	add(angleMat, Scalar(180), angleMat, angleMat<0);                //���angleMat<0�����180
	add(angleMat, Scalar(-180), angleMat, angleMat >= 180);          //���angleMat>=180�����180
	//��4���������д��뽫�ǶȾ���ת��Ϊһ���Ҷ�ֵ��Χ��[0,9]֮���ͼ��
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
	//��5���������ѭ������ʵ�ǽ�ͼ����ݶȷ�ֵ���󰴾Ÿ���ͬ������ݶȽǶȣ���ÿ���Ƕȷ�Χ����Ӧ����ݶȷ�ֵ
	//     �洢����Ӧ�ľ���ͼ��֮�ϣ���ʵ���ǽ��ݶȷ�ֵ����ͼ���ղ�ͬ���ݶȷ�ֵ�Ƕȷ�Ϊ9���ݶȷ�ֵ��ͼ��
	// 9���ݶȷ�ֵͼ��
	for (int y = 0; y < srcMat.rows; y++)
	{
		for (int x = 0; x < srcMat.cols; x++)
		{
			int ind = angleMat.at<float>(y, x); // ind��ʾ��ǰ�����ݶ��ڵڼ���bin��
			bins[ind].at<float>(y, x) += magnMat.at<float>(y, x);
		}
	}
	//��6�������������ɵ�9�Ų�ͬ�Ƕȵ��ݶȷ�ֵ��������9�Ų�ͬ���ݶȷ�ֵ�Ļ���ͼ�������Ժ�
	//     ����ͼ���ÿһ��ʹ�����һ�����Ͻ������ݶȷ�ֵ֮�ͣ����ɵ�9������ͼҲ����9��
	//     bins����ͬbins�ϵ�HOGǿ��
	std::vector<Mat> integrals(NBINS);
	for (int i = 0; i < NBINS; i++)
	{
		integral(bins[i], integrals[i]);
	}
	//����ͼ��ʲô�����¿�
	return integrals;
}
/********************************************************************************************************
��������:
���㵥��cell HOG����
����˵��:
1)cv::Mat& HOGCellMat-------------�洢ÿ��cellHOG����������������
2)cv::Rect roi--------------------����cell�ľ���λ��
3)std::vector<Mat>& integrals-----�洢��9������ͼ��ÿһ������ͼ�����һ���Ƕȷ�Χ����һ��bins
*********************************************************************************************************/
void cacHOGinCell(cv::Mat& HOGCellMat, cv::Rect roi, std::vector<Mat>& integrals)
{
	//��1��ͨ��9������ͼ�����ʵ��HOG�ļ��㣬HOG���ֱ��ͼ��9��bins��ÿ��bins�Ͷ�Ӧһ�Ż���ͼ��
	int x0 = roi.x;                              //ȷ����������cell�����Ͻǵ�����
	int y0 = roi.y;
	int x1 = x0 + roi.width;
	int y1 = y0 + roi.height;                    //ȷ����������cell�����½ǵ�����

	for (int i = 0; i <NBINS; i++)
	{
		//��2�����ݾ��ε����Ͻǵ�����½ǵ������
		cv::Mat integral = integrals[i];

		float a = integral.at<double>(y0, x0);
		float b = integral.at<double>(y1, x1);
		float c = integral.at<double>(y0, x1);
		float d = integral.at<double>(y1, x0);

		HOGCellMat.at<float>(0, i) = b - c - d + a;//ÿѭ��һ�Σ�����һ���ݶȷ����ϵ�HOG��������ʵ����
		//ÿѭ��һ�Σ��ͼ����ݶȷ���ֱ��ͼ�ϵ�һ��bins
	}
}
/****
��ȡ��ǰ���ڵ�HOGֱ��ͼ----�˿���ʵ�����ڼ��㵥��Block��HOG�ݶȷ���ֱ��ͼ
1)cv::Point pt--------------------����Block�����ĵ�����
2)std::vector<cv::Mat>& integrals-----�洢��9������ͼ��ÿһ������ͼ�����һ���Ƕȷ�Χ����һ��bins
**********/
cv::Mat getHog(cv::Point pt, std::vector<cv::Mat>& integrals)
{
	if (pt.x - R<0 || pt.y - R<0 || pt.x + R >= integrals[0].cols || pt.y + R >= integrals[0].rows)
	{
		return cv::Mat();
	}
	//��1��BLOCK��HOGֱ��ͼ---�������˵��BLOCKSIZE*BLOCKSIZE��4��cell��HOG����ֱ��ͼ��������
	//     ���һ��BLOCK��HOG����ֱ��ͼ����������
	cv::Mat    hist(cv::Size(NBINS*BLOCKSIZE*BLOCKSIZE, 1), CV_32F);
	cv::Point  t1(0, pt.y - R);
	int c = 0;
	//��2��������:ͨ������������ѭ�����ͱ�����4��cell�����ҽ�4��cell��HOG�������������һ��
	//     ά���Ƚϴ��BLOCK��HOG��������
	for (int i = 0; i<BLOCKSIZE; i++)
	{
		t1.x = pt.x - R;
		for (int j = 0; j<BLOCKSIZE; j++)
		{
			//��3����ȡ��ǰ���ڣ����оֲ�HOGֱ��ͼ����
			cv::Rect roi(t1, t1 + cv::Point(CELLSIZE, CELLSIZE));
			cv::Mat  hist_temp = hist.colRange(c, c + NBINS);
			//��4������roiȷ���ľ������򣬼��㵥��cell��HOGֱ��ͼ(�䱾�ʾ���һ������������)
			cacHOGinCell(hist_temp, roi, integrals);
			t1.x += CELLSIZE;
			c += NBINS;
		}
		t1.y = CELLSIZE;
	}//for i
	//��3�����÷���2���й�һ��
	cv::normalize(hist, hist, 1, 0, NORM_L2);
	return hist;
}

//��������ͼ���HOG�ݶȷ���ֱ��ͼ---HOG��������������ͼHOG������������ָ��������
Mat cacHOGFeature(cv::Mat srcImage, string srcImgPath)
{
	cv::Mat          grayImage;
	
	cv::cvtColor(srcImage, grayImage, CV_RGB2GRAY);
	grayImage.convertTo(grayImage, CV_8UC1);// ��ɵ�ͨ���ĻҶ�ͼ
	
	 std::vector<Mat> HOGFeatureVector;
	//���ﶨ��ȫ�ֵ�HOGMatVector��������svmtrain�л�ȡ���С
	//HOGMatVector.clear();
	 cout << " �����ڣ�" << HOGFeatureVector.size() << endl;
	
	//��1��9����ͬ�ݶȷ����ϵ�9���ݶȷ�ֵ�Ļ���ͼ�������
	std::vector<Mat> integrals = CalculateIntegralHOG(grayImage);
	Mat image = grayImage.clone();   //�ڻҶ�ͼ�ϻ���HOG�������ӻ�
	//Mat image(srcImage.rows, srcImage.cols, CV_8UC4);
	image *= 0.5;
	//��2������ȫͼ�񣬼������յ��ݶȷ���ֱ��ͼHOG
	cv::Mat HOGBlockMat(Size(NBINS, 1), CV_32F);
	for (int y = CELLSIZE / 2; y < grayImage.rows; y += CELLSIZE)
	{
		for (int x = CELLSIZE / 2; x < grayImage.cols; x += CELLSIZE)
		{
			//��3����ȡ��ǰ����HOG����ʵ��ǰ�Ĵ��ھ���һ��Block��ÿ��Block��4��cell��ɣ�ÿ��CellΪ20*20
			//     �˿飬����ľ��ǵ���Block���ݶȷ���ֱ��ͼHOG
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
			//��4��L2������һ��:����õ���ÿ��Block�ĵľ������L2������һ����ʹ��ת��Ϊһ��Block��HOG��������
			normalize(HOGBlockMat, HOGBlockMat, 1, 0, CV_L2);
			//��5�����ÿ�õ�һ��Block��HOG���������ʹ���HOGMatVector�����HOGMatVector��ʵ��������ͼ���HOG����������
			//     ��Ȼ���������HOGMatVector���Ǹ���ά�������ʽ�������Ҫ����SVM������з���Ļ�������Ҫ��������Ϊһ
			//     ά��������
			HOGFeatureVector.push_back(HOGBlockMat);
			Point center(x, y);
			//��6������HOG����ͼ
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
	cout << "��ԭ�Ҷ�ͼ����ʾHOG����ͼ��" << endl;
	imshow("HOG�������ӻ���", image);
	BLOCKNUM = HOGFeatureVector.size();
	cout << "�����ڣ�" << HOGFeatureVector.size() << endl;
	//waitKey(0);
	// HOGFeatureVectorת����HOGFeatureMat�洢
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
	return HOGFeatureMat; //����ÿ��ͼ���������������
}

// MLP function

//�õ�����ѵ�����߲��Ե���������
Mat getTrainTestHOGMat(Mat HOGMat, string train_test, int classNum, int start, int end, bool test )
{
	int smpW = FEATURE_DIM;// NBINS * BLOCKNUM;
	int smpH = 1;
	// ׼����ѵ����HOG������������
	//Mat HOGMat(TRAIN_IMG_ALL, smpW * smpH, CV_32FC1);
	for (int i = 0; i < classNum; i++)
	{
		for (int j = start; j < end; j++)
		{
			string path = format("images\\srcImg\\charData\\%s\\%d%d.png", train_test.c_str(), i, j);
			string name = format("charData\\%s\\%d%d.png", train_test.c_str(), i, j);
			cout << name << endl;
			imgResize(path);
			Mat image = imread(path);
			if (image.empty())
			{
				cout << "û�ж�ȡ��ͼ��" << endl;
				return Mat();
			}
				
			imshow(name, image);
		//	waitKey(0);
			Mat HOGFeatureMat = cacHOGFeature(image, name);
			cout << "��ȡ����HOGMatVector,vector��СΪ��" << BLOCKNUM << endl;
			int index;
			if (train_test == "train")
			{
				index = i * (end - start) + j - start;
				float* Data = HOGMat.ptr<float>(index);
				cout << "����ȡ��" << index << "��ͼ���HOG����������������ѵ��������" << endl;
			}
			if (train_test == "test")
			{
				index = 0;
				float* Data = HOGMat.ptr<float>(index);
				cout << "ʶ�������ͼ�������" << endl;
			}
		//	cout << "---------------------------" << endl;
			for (int n = 0; n < FEATURE_DIM; n++)
			{
				HOGMat.at<float>(index, n) = HOGFeatureMat.at<float>(0, n);
			}
			if (test)
			{
				// SVM_test(HOGMat);
				ANN_MLP_test(HOGMat);
			}
		}
	}
	return HOGMat;
}

int SVM_train(Mat trainHOGMat)
{
	//ѵ�� CLASS_NUM �����ݣ�ÿ�� EVERY_CLASS_NUM ��
	int labels[TRAIN_IMG_SVM] = { 0, 0, 0, 0, 1, 1, 1, 1 };
	Mat labelsMat(TRAIN_IMG_SVM, 1, CV_32SC1, labels);  //��label��ɾ������ʽ����������ҪΪCV_32SC1
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 1000, 1e-6)); //ѵ����ֹ����

	//����ѵ������
	Ptr<TrainData> train_data = TrainData::create(trainHOGMat, ROW_SAMPLE, labelsMat);
	cout << "��ʼѵ��" << endl;
	svm->train(train_data);
	cout << "ѵ������������ģ��ΪhogSvm.xml." << endl;
	svm->save("hogSvm.xml");

	return 0;
}

int SVM_test(Mat testHOGMat)
{
	cout << "����SVMģ��hogSvm.xml." << endl;
	svm = SVM::load<SVM>("hogSvm.xml");

	//���Ԥ�����ľ���
	Mat predictionMat(1, 1, CV_32S);
	float response = svm->predict(testHOGMat, predictionMat);
	float* data = predictionMat.ptr<float>(0);
	cout << "Ԥ����Ϊ" << data[0] << endl;
	cout << "svm test" << endl;
	return 0;
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
				// ͼ���С���� FEATURESIZE �໥����
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
	};  // ÿ���������ݶ�Ӧ�����	
	// TODO 
	// ��Ϊ���ļ��ж�ȡlabel��
	Mat labelsMat(120, 3, CV_32FC1, labels);  // ԭʼΪ8��1�������� ������MLP�����ͬ
	cout << "��ǩ����Ϊ��" << endl;
	cout << labelsMat << endl;

	cout << "����MLP������" << endl;
	Ptr<ANN_MLP> bp = ANN_MLP::create();
	Mat layerSizes = (Mat_<int>(1, 3) << FEATURE_DIM, 6, 3);
	bp->setLayerSizes(layerSizes);
	bp->setTrainMethod(ANN_MLP::BACKPROP, 0.1, 0.1);
	bp->setActivationFunction(ANN_MLP::SIGMOID_SYM);
	bp->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 1000, 1e-6));
	cout << "ѵ��������ing..." << endl;
	bool trained = bp->train(trainMat, ROW_SAMPLE, labelsMat);
	if (trained)
	{
		bp->save("ANN_MLP.xml");
		cout << "����ѵ���õ�ģ��ΪANN_MLP.xml" << endl;
	}
	return 0;
}

int ANN_MLP_test(Mat testMat)
{
	Ptr<ANN_MLP> bp = ANN_MLP::create();
	cout << "����Ԥģ���ļ� ANN_MLP.xml ..." << endl;
	bp = ANN_MLP::load<ANN_MLP>("ANN_MLP.xml");   //����ѵ���õ�ģ��
	/*cout << bp.empty() << endl;
	if (! bp.empty())
	{
		cout << "����Ԥѵ��ģ��ʧ�ܣ�����ѵ��" << endl;
		return -1;
	}*/
	cout << "�������" << endl;
	Mat predictionMat(1, 3, CV_32FC1);
	float response = bp->predict(testMat, predictionMat);  // �������⣬������ʾʶ����
	float* data = predictionMat.ptr<float>(0); //data[0]ȡ����һ�е�һ����
	cout << "Ԥ�����" << predictionMat << endl;	
	//�����������������һ�����ж����ĸ����
	//Ӧ�ü���һ����ֵ��0.5��������ֵʱ��δ��⵽�κ�Ŀ��
	//float max = data[0];
	//int tmp = 0;
	//for (int i = 0; i < predictionMat.cols; i++)
	//{
	//	if (data[i]>max)
	//	{
	//		max = data[i];
	//		tmp = i;
	//	}
	//}
	//if (max > 0.2)
	//{
	//	switch (tmp)
	//	{
	//		case 0: cout << "Ԥ����Ϊ��" << "��0��," << "����Ϊ" << max << endl;  break;
	//		case 1: cout << "Ԥ����Ϊ��" << "��1��," << "����Ϊ" << max << endl;  break;
	//		case 2: cout << "Ԥ����Ϊ��" << "��2��," << "����Ϊ" << max << endl;  break;
	//		default: break;
	//	}
	//}
	//else
	//	cout << "error δ��⵽�κ����" << endl;
	//cout << "*****************************" << endl;
	////waitKey(0);
	//int classNum = tmp;
	int classNumResault = getTestResault(predictionMat, data);
	return classNumResault;
}

int getTestResault(Mat predictionMat,float* data)
{
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
	if (max > 0.2)
	{
		switch (tmp)
		{
		case 0: cout << "Ԥ����Ϊ��" << "��0��," << "����Ϊ" << max << endl;  break;
		case 1: cout << "Ԥ����Ϊ��" << "��1��," << "����Ϊ" << max << endl;  break;
		case 2: cout << "Ԥ����Ϊ��" << "��2��," << "����Ϊ" << max << endl;  break;
		default: break;
		}
	}
	else
		cout << "error δ��⵽�κ����" << endl;
	cout << "*****************************" << endl;
	//waitKey(0);
	return tmp;
}

int main()
{
/*��1��ѵ��*/
	//����HOG�����������Ŀ��
	cout << "׼��������..." << endl;
	//// ׼����ѵ����HOG������������
	//Mat trainHOGMat(TRAIN_IMG_SVM, FEATURE_DIM, CV_32FC1);
	//Mat testHOGMat(1, FEATURE_DIM, CV_32FC1);
	//// ��ԭʼͼ����ȡHOG��������
	//trainHOGMat = getTrainTestHOGMat(trainHOGMat, "train", CLASS_NUM, 1, 5,false);
	//cout << "����׼����ϣ�׼��ѵ��SVM..." << endl;
	//SVM_train(trainHOGMat);
	//testHOGMat = getTrainTestHOGMat(testHOGMat, "test", CLASS_NUM, 5, 6,true);
	//SVM_test(testHOGMat);  //ÿ���һ��ͼ�����Ҫ���

	/*  ANN  */
	Mat trainHOGMatANN(TRAIN_IMG_ANN, FEATURE_DIM, CV_32FC1);
	Mat testHOGMatANN(1, FEATURE_DIM, CV_32FC1);

	//trainHOGMatANN = getTrainTestHOGMat(trainHOGMatANN, "train", 3, 1, 41,false);
	cout << "ANN ѵ������׼�����" << endl;
	//ANN_MLP_train(trainHOGMatANN);
	testHOGMatANN = getTrainTestHOGMat(testHOGMatANN, "test", 3, 41, 50, true);

	cout << "�������" << endl;
	waitKey();
	return 0;
}

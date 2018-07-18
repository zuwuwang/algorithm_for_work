
#include <string>
#include <iostream>
#include <vector> 

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

//����ԭʼ����ͼ��Ŀ��
const int imgWidth = 14;
const int imgHigh = 23;

int SVM_TEST()
{
	//ѵ��2�����ݣ�ÿ��4��
	int labels[8] = { -1, -1, -1, -1, 1, 1, 1, 1 };
	Mat labelsMat(8, 1, CV_32SC1, labels);  //��label��ɾ������ʽ

	//����������ߣ���ԭʼ������ؾ���
	int smpW = imgWidth;
	int smpH = imgHigh;

	//��������ֵ�����ظ���ѵ��ͼ��
	printf("start to parpare traindata...\n");
	Mat trainMat(8, smpW * smpH, CV_32FC1);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 1; j < 5; j++)
		{
			string path = format("data\\%d%d.png", i, j);
			Mat img = imread(path, 0);
			//Canny(img, img, 50, 100);

			int index1 = i * 4 + j - 1;
			float * trainData = trainMat.ptr<float>(index1);

			for (int h = 0; h < smpH; h++)
			{
				uchar * imgData = img.ptr<uchar>(h);
				for (int w = 0; w < smpW; w++)
				{
					int index2 = h*smpW + w;
					trainData[index2] = imgData[w];
				}
			}
		}
	}

	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));

	//����ѵ������ 
	Ptr<TrainData> tData = TrainData::create(trainMat, ROW_SAMPLE, labelsMat);
	cout << "start to train svm..." << endl;;
	svm->train(tData);
	cout << "train over,save svm.xml file" << endl;;
	svm->save("svm.xml");


	//����ѵ���õ�ģ�Ͳ�������
	//cout<<"load svm.xml file to test ..."<<endl;
	//Ptr<SVM> svm = SVM::load("data.xml");

	for (int i = 0; i < 2; i++)
	{
		for (int j = 5; j < 6; j++)
		{
			Mat testMat(1, smpW * smpH, CV_32FC1);
			float * testData = testMat.ptr<float>(0);

			string path = format("data\\%d%d.png", i, j);
			Mat img = imread(path, 0);
			//Canny(img, img, 50, 100);

			for (int h = 0; h < smpH; h++)
			{
				uchar * imgData = img.ptr<uchar>(h);  
				for (int w = 0; w < smpW; w++)
				{
					int index2 = h*smpW + w;
					testData[index2] = imgData[w] * 1.0;  //��������
				}
			}

			Mat ret(1, 1, CV_32S);
			cout<<" SVM predict resault is "<<endl;
			float response = svm->predict(testMat, ret);  // ����ѵ���õ�ģ���Ͻ���Ԥ��
										 // ����1��-1
			
			cout << response << endl;
			float * data = ret.ptr<float>(0);
			cout << data[0] << endl;
			waitKey(10);
		}
	}
	return 0;
}


int ANN_TEST()
{
	// 14 x 23 ������ԭʼͼ���С����
	int smpW = imgWidth;
	int smpH = imgHigh;

	float labels[8] = { 0, 0, 0, 0, 1, 1, 1, 1 };  // ÿ���������ݶ�Ӧ�����	
	Mat labelsMat(8, 1, CV_32FC1, labels);
	cout << "��ǩ����Ϊ��" << endl;
	cout << labelsMat << endl;

	// ѵ������
	Mat trainMat(8, smpW * smpH, CV_32FC1);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 1; j < 5; j++)
		{
			string path = format("data\\%d%d.png", i, j);
			Mat img = imread(path, 0);
			if (img.empty())
			{
				return 0;
			}
			//Canny(img, img, 50, 100);

			int index1 = i * 4 + j - 1;
			float * trainData = trainMat.ptr<float>(index1);

			for (int h = 0; h < smpH; h++)
			{
				uchar * imgData = img.ptr<uchar>(h);
				for (int w = 0; w < smpW; w++)
				{
					int index2 = h*smpW + w;
					trainData[index2] = imgData[w];
				}
			}
		}
	}

	// BP ģ�ʹ����Ͳ�������
	Ptr<ANN_MLP> bp = ANN_MLP::create();

	// ���岢���ò�Ĵ�С
	Mat layerSizes = (Mat_<int>(1, 3) << smpW * smpH, 6, 1);// ����Ϊͼ���С
													  // �������1���ڵ㣬��Ӧ�����࣬labelΪ����
													  // �м����ز�
	bp->setLayerSizes(layerSizes);
	
	// �����������
	bp->setTrainMethod(ANN_MLP::BACKPROP, 0.1, 0.1);  // ���򴫲���ѵ������
												// ����bp_dw_scale��bp_moment_scale��������
												// �ο����� https://blog.csdn.net/xiaowei_cqu/article/details/9027617
	bp->setActivationFunction(ANN_MLP::SIGMOID_SYM);  //��sigmoid����Ϊ�����
	bp->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 10000, /*FLT_EPSILON*/1e-6));

	// ����ѵ���õ����������
	bool trained = bp->train(trainMat, ROW_SAMPLE, labelsMat);
	if (trained) 
	{
		cout << "save bp_param.xml" << endl;
		bp->save("bp_param.xml");
	}


	//����ѵ���õ�������
	//printf("load bp_param.xml file to predict...\n");
	//Ptr<ANN_MLP> bp = ANN_MLP::load("bp_param.xml");   //�ظ������ˣ�ѵ������ֻ��Ҫһ��

	for (int i = 0; i < 2; i++)
	{
		for (int j = 5; j < 6; j++)  // ��05 ��15����ͼ�����Ԥ��
		{
			Mat testMat(1, smpW * smpH, CV_32FC1);
			float * testData = testMat.ptr<float>(0);

			string path = format("data\\%d%d.png", i, j);
			Mat img = imread(path, 0);
			//Canny(img, img, 50, 100);

			for (int h = 0; h < smpH; h++)
			{
				uchar * imgData = img.ptr<uchar>(h);
				for (int w = 0; w < smpW; w++)
				{
					int index2 = h*smpW + w;
					testData[index2] = imgData[w] * 1.0;
				}
			}

			Mat responseMat; //Ԥ��Ľ������
			bp->predict(testMat, responseMat);
			float response = responseMat.ptr<float>(0)[0];
			cout << response << endl;
			float * data = responseMat.ptr<float>(0);
			cout << data[0] << endl;
			waitKey(10);
		}
	}
	waitKey(0);
	return 0;
}

int imgResize()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 1; j < 8; j++)
		{
			string path = format("data\\%d%d.png", i, j);
			Mat img = imread(path, 0);
			if (img.empty())
			{
				return 0;
			}
			else
			{
				resize(img, img, Size(imgWidth, imgHigh));
				imwrite(path, img);
			}
		}
	}
	return 0;
}
int main()
{
	// ��������ͼ�񣬲�������ͼ��resizeΪ�ƶ���С������
	imgResize();
	// ��ȡHOG����������HOG����ͼ��
	// TODO

	// ����HOGͼ�����ѵ���ͷ������
	// TODO

	// ��resize���ͼ������SVM��MLP�з���ѵ��������������
	SVM_TEST();
	ANN_TEST();
	system("pause");
}

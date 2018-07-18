
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

//定义原始输入图像的宽高
const int imgWidth = 14;
const int imgHigh = 23;

int SVM_TEST()
{
	//训练2类数据，每类4张
	int labels[8] = { -1, -1, -1, -1, 1, 1, 1, 1 };
	Mat labelsMat(8, 1, CV_32SC1, labels);  //将label变成矩阵的形式

	//设置样本宽高，由原始相机像素决定
	int smpW = imgWidth;
	int smpH = imgHigh;

	//将样本的值逐像素赋给训练图像
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

	//设置训练数据 
	Ptr<TrainData> tData = TrainData::create(trainMat, ROW_SAMPLE, labelsMat);
	cout << "start to train svm..." << endl;;
	svm->train(tData);
	cout << "train over,save svm.xml file" << endl;;
	svm->save("svm.xml");


	//根据训练好的模型测试数据
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
					testData[index2] = imgData[w] * 1.0;  //测试数据
				}
			}

			Mat ret(1, 1, CV_32S);
			cout<<" SVM predict resault is "<<endl;
			float response = svm->predict(testMat, ret);  // 根据训练好的模型上进行预测
										 // 返回1或-1
			
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
	// 14 x 23 矩阵，由原始图像大小决定
	int smpW = imgWidth;
	int smpH = imgHigh;

	float labels[8] = { 0, 0, 0, 0, 1, 1, 1, 1 };  // 每个样本数据对应的输出	
	Mat labelsMat(8, 1, CV_32FC1, labels);
	cout << "标签矩阵为：" << endl;
	cout << labelsMat << endl;

	// 训练样本
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

	// BP 模型创建和参数设置
	Ptr<ANN_MLP> bp = ANN_MLP::create();

	// 定义并设置层的大小
	Mat layerSizes = (Mat_<int>(1, 3) << smpW * smpH, 6, 1);// 输入为图像大小
													  // 输出层有1个节点，对应两个类，label为两类
													  // 中间隐藏层
	bp->setLayerSizes(layerSizes);
	
	// 设置网络参数
	bp->setTrainMethod(ANN_MLP::BACKPROP, 0.1, 0.1);  // 反向传播的训练方法
												// 设置bp_dw_scale，bp_moment_scale两个参数
												// 参考链接 https://blog.csdn.net/xiaowei_cqu/article/details/9027617
	bp->setActivationFunction(ANN_MLP::SIGMOID_SYM);  //以sigmoid函数为激活函数
	bp->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 10000, /*FLT_EPSILON*/1e-6));

	// 保存训练好的神经网络参数
	bool trained = bp->train(trainMat, ROW_SAMPLE, labelsMat);
	if (trained) 
	{
		cout << "save bp_param.xml" << endl;
		bp->save("bp_param.xml");
	}


	//创建训练好的神经网络
	//printf("load bp_param.xml file to predict...\n");
	//Ptr<ANN_MLP> bp = ANN_MLP::load("bp_param.xml");   //重复定义了，训练测试只需要一个

	for (int i = 0; i < 2; i++)
	{
		for (int j = 5; j < 6; j++)  // 对05 、15两张图像进行预测
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

			Mat responseMat; //预测的结果矩阵
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
	// 读入所有图像，并把所有图像resize为制定大小并保存
	imgResize();
	// 提取HOG特征，保存HOG特征图像
	// TODO

	// 读入HOG图像进行训练和分类测试
	// TODO

	// 将resize后的图像送入SVM、MLP中分类训练，并输出检测结果
	SVM_TEST();
	ANN_TEST();
	system("pause");
}

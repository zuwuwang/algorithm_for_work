
#include <string>
#include <iostream>
#include <vector> 

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

//定义原始输入图像的宽高
const int imgWidth = 14;
const int imgHigh = 23;
//const int classNum = 2;  //训练的类别数
//const int trainEndNum = 5;
//const int testEndNum = 6;

// debug 模式下报错 
//const string dataPath = format("charData");  // 指定数据集的路径，or TowerData instead

int SVM_TEST()
{
	//训练2类数据，每类4张，指明图像的类别
	int labels[8] = { 0, 0, 0, 0, 1, 1, 1, 1 };
	Mat labelsMat(8, 1, CV_32SC1, labels);  //将label变成矩阵的形式，数据类型要为CV_32SC1

	//设置样本宽高，由原始相机像素决定
	int smpW = imgWidth;
	int smpH = imgHigh;

	//将样本的值逐像素赋给训练图像
	cout << "start to parpare traindata...\n" << endl;
	Mat trainMat(8, smpW * smpH, CV_32FC1);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 1; j < 5; j++)
		{
			string path = format("charData\\%d%d.png", i, j);
			Mat img = imread(path, 0);
			//Canny(img, img, 50, 100);
			if (img.empty())
			{
				break;
			}
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
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 1000, 1e-6));

	//设置训练数据 
	Ptr<TrainData> tData = TrainData::create(trainMat, ROW_SAMPLE, labelsMat);
	cout << "start to train svm..." << endl;
	svm->train(tData);
	cout << "train over,save svm.xml file" << endl;;
	svm->save("svm.xml");


	//载入训练好的模型
	cout<<"load svm.xml file to test ..."<<endl;
	svm = SVM::load<SVM>("svm.xml");
	
	//载入测试数据
	for (int i = 0; i < 2; i++)
	{
		for (int j = 5; j < 6; j++)
		{
			//一次读入一张图像进行test
			Mat testMat(1, smpW * smpH, CV_32FC1);
			float * testData = testMat.ptr<float>(0);

			string path = format("charData\\%d%d.png",i, j);
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

			//预测
			Mat predictionMat(1, 1, CV_32S);
			cout<<" SVM predict resault is "<<endl;
			float response = svm->predict(testMat, predictionMat);  // 根据训练好的模型上进行预测，返回值是一个无用的float值，忽略
			cout << "predict函数执行返回值: " << response << "\t<0为成功，否则失败!>" << endl;
			float * data = predictionMat.ptr<float>(0); //识别结果，输出为该图像所对应的标签类别
			cout << "正在识别的图像是：" << i << j << ".png" << endl;
			cout << "预测结果为 "<< data[0]<<"\n" << endl;
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

	//float labels[12][1] = { 0, 0, 0, 0, 1, 1, 1, 1, 2 ,2, 2, 2 };  // 每个样本数据对应的输出	
	float labels[12][3] = { { 1, 0, 0 }, { 1, 0, 0 }, {1,0,0},
					      { 0, 1, 0 }, { 0, 1, 0 }, {0,1,0},
						  { 0, 0, 1 }, { 0, 0, 1 }, {0,0,1}
						};  // 每个样本数据对应的输出	
	Mat labelsMat(12, 3, CV_32FC1, labels);  // 原始为8，1，行数与 列数与MLP输出相同
	cout << "标签矩阵为：" << endl;
	cout << labelsMat << endl;

	// 将单张图像取出，一次放入trainMat待训练样本中
	Mat trainMat(12, smpW * smpH, CV_32FC1);
	//遍历所有待训练图像
	for (int i = 0; i < 3; i++)
	{
		for (int j = 1; j < 5; j++)
		{
			//读入图像
			string path = format("charData\\%d%d.png", i, j);
			cout << path << endl;
			Mat img = imread(path, 0);
			if (img.empty())
			{
				break;
			}
			//Canny(img, img, 50, 100);

			//每个读入的图像在trainMat中的索引
			int index1 = i * 4 + j - 1;
			float* trainData = trainMat.ptr<float>(index1);
			//对imread进来的图像进行处理，存入trainMat中
			//以trainData作为整个
			for (int h = 0; h < smpH; h++)
			{
				uchar * imgData = img.ptr<uchar>(h);
				for (int w = 0; w < smpW; w++)
				{
					//图像的每行转换成为Mat的一行中的下一操作位置
					int index2 = h*smpW + w;
					trainData[index2] = imgData[w];
				}
			}
		}
	}
	
	// BP 模型创建和参数设置
	Ptr<ANN_MLP> bp = ANN_MLP::create();
	
	// 定义并设置层的大小
	// TODO ：更改输出类别数，输出4分类结果
	Mat layerSizes = (Mat_<int>(1, 3) << smpW * smpH, 6, 3);// 输入为图像大小
													  // 中间隐藏层，默认为6
													 // 输出层有几类就设置几个节点，默认是1个
	bp->setLayerSizes(layerSizes);
	
	// 设置网络参数
	bp->setTrainMethod(ANN_MLP::BACKPROP, 0.1, 0.1);  // 反向传播的训练方法
												// 设置bp_dw_scale，bp_moment_scale两个参数
												// 参考链接 https://blog.csdn.net/xiaowei_cqu/article/details/9027617
	bp->setActivationFunction(ANN_MLP::SIGMOID_SYM);  //以sigmoid函数为激活函数
	bp->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 10000, /*FLT_EPSILON*/1e-6));

	// 保存训练好的神经网络参数
	bool trained = bp->train(trainMat, ROW_SAMPLE, labelsMat);  //ANN_MLP类继承自StatModel模型，方法train()通过函数参数重载
														  // bool train( InputArray samples, int layout, InputArray responses )
														 // 这里labelMat的行和列分别与输入输出的维度相同
	if (trained) 
	{
		cout << "save bp_param.xml" << endl;
		bp->save("bp_param.xml");
	}


	//创建训练好的神经网络
	cout << "load bp_param.xml file ..." << endl;
	bp = ANN_MLP::load<ANN_MLP>("bp_param.xml");   //加载训练好的模型
	cout << "load bp_param.xml ok!" << endl;

	//读入测试数据，方法与读入待训练数据相同
	for (int i = 0; i < 3; i++)
	{
		for (int j = 5; j < 6; j++)  // 对05 、15两张图像进行预测
		{
			//因为只是测试一张图像，testMat第一个元素为待测试的图像数量
			//这里可以按上面的index1定义一个索引，或者直接用count++索引
			Mat testMat(1, smpW * smpH, CV_32FC1);
			
			// int index1 = i * 2 + j - 5;
			float * testData = testMat.ptr<float>(0);

			string path = format("charData\\%d%d.png", i, j);
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
			//对于每个读入的数据，使用分类器做一个预测
			Mat predictionMat(1,3,CV_32FC1);
			cout << "MLP predict is" << endl;
			float response = bp->predict(testMat, predictionMat);  // 分类问题，则误差表示识别率
											// 该方法位于statModel统计模型中，继承与Algorithm类
										    // testMat是提取的特征，作为输入；response为一个可以选择的输出矩阵，1*n维度，n为类别；flag是一个可选标志位
											// 输出的这个结果矩阵表示什么？表示册数图像是目标的概率
											// 返回值是一个无用的float值，忽略
		//	float response = responseMat.ptr<float>(0)[0]; // Mat.ptr直接得到一行的指针，这里取出第一行的第一个数，error，要着没用	
		//	cout << "预测函数返回值response: " << response << "\t<0为成功，否则失败!>" << endl;
			float * data = predictionMat.ptr<float>(0); //data[0]取出第一行第一个数
			cout << "正在识别的图像是：" << "第"<<i<<"类"<<"第" << j << "张" << endl;
			cout << "预测矩阵："<< predictionMat << endl;
			//cout << "预测概率 data[0]:"<< data[0] <<"\n"<< endl;
			
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
			if (max > 0.5)
			{
				switch (tmp)
				{
				case 0: cout << "预测结果为：" << "第0类," << "概率为" << max << endl; break;
				case 1: cout << "预测结果为：" << "第1类," << "概率为" << max << endl;  break;
				case 2: cout << "预测结果为：" << "第2类," << "概率为" << max << endl;  break;
				default: break;
				}
			}
			else
			{
				cout << "error 未检测到任何类别！" << endl;
			}
			cout << "*****************************" << endl;
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
			string path = format("charData\\%d%d.png", i, j);
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
// GPS数据读取，计算方位距离

// 目标检测
	//训练
	// 读入所有图像，并把所有图像resize为制定大小并保存
	imgResize();

	// 提取HOG特征，保存HOG特征图像计算梯度直方图
	// OK

	// 读入HOG图像进行训练
	// OK

	// 将resize后的图像送入SVM、MLP中分类训练，并输出检测结果
	// 分类器测试
	SVM_TEST();  // 做背景、杆塔的分类，提取ROI区域，输入ANN中
	ANN_TEST();  // 做姿态的分类，先做四分类，正面、侧45°、侧面、塔底

  //检测
	// 以滑窗形式抽取矩形区域图像
	// TODOhttps://docs.opencv.org/3.1.0/d0/dce/classcv_1_1ml_1_1ANN__MLP.png

	// 抽取图像区域输入到目标检测分类器中分类，识别目标，得到完备的ROI区域
	// TODO

	// 将ROI区域的HOG特征图输入到姿态检测分类器中分类，得到姿态分类结果
	// TODO

	// 根据分类结果，反馈回无人机做飞控参数调整
	// 给出方案，模拟仿真
	system("pause");
	return 0;
}

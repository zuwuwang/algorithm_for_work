
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

//����ԭʼ����ͼ��Ŀ��
const int imgWidth = 14;
const int imgHigh = 23;
//const int classNum = 2;  //ѵ���������
//const int trainEndNum = 5;
//const int testEndNum = 6;

// debug ģʽ�±��� 
//const string dataPath = format("charData");  // ָ�����ݼ���·����or TowerData instead

int SVM_TEST()
{
	//ѵ��2�����ݣ�ÿ��4�ţ�ָ��ͼ������
	int labels[8] = { 0, 0, 0, 0, 1, 1, 1, 1 };
	Mat labelsMat(8, 1, CV_32SC1, labels);  //��label��ɾ������ʽ����������ҪΪCV_32SC1

	//����������ߣ���ԭʼ������ؾ���
	int smpW = imgWidth;
	int smpH = imgHigh;

	//��������ֵ�����ظ���ѵ��ͼ��
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

	//����ѵ������ 
	Ptr<TrainData> tData = TrainData::create(trainMat, ROW_SAMPLE, labelsMat);
	cout << "start to train svm..." << endl;
	svm->train(tData);
	cout << "train over,save svm.xml file" << endl;;
	svm->save("svm.xml");


	//����ѵ���õ�ģ��
	cout<<"load svm.xml file to test ..."<<endl;
	svm = SVM::load<SVM>("svm.xml");
	
	//�����������
	for (int i = 0; i < 2; i++)
	{
		for (int j = 5; j < 6; j++)
		{
			//һ�ζ���һ��ͼ�����test
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
					testData[index2] = imgData[w] * 1.0;  //��������
				}
			}

			//Ԥ��
			Mat predictionMat(1, 1, CV_32S);
			cout<<" SVM predict resault is "<<endl;
			float response = svm->predict(testMat, predictionMat);  // ����ѵ���õ�ģ���Ͻ���Ԥ�⣬����ֵ��һ�����õ�floatֵ������
			cout << "predict����ִ�з���ֵ: " << response << "\t<0Ϊ�ɹ�������ʧ��!>" << endl;
			float * data = predictionMat.ptr<float>(0); //ʶ���������Ϊ��ͼ������Ӧ�ı�ǩ���
			cout << "����ʶ���ͼ���ǣ�" << i << j << ".png" << endl;
			cout << "Ԥ����Ϊ "<< data[0]<<"\n" << endl;
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

	//float labels[12][1] = { 0, 0, 0, 0, 1, 1, 1, 1, 2 ,2, 2, 2 };  // ÿ���������ݶ�Ӧ�����	
	float labels[12][3] = { { 1, 0, 0 }, { 1, 0, 0 }, {1,0,0},
					      { 0, 1, 0 }, { 0, 1, 0 }, {0,1,0},
						  { 0, 0, 1 }, { 0, 0, 1 }, {0,0,1}
						};  // ÿ���������ݶ�Ӧ�����	
	Mat labelsMat(12, 3, CV_32FC1, labels);  // ԭʼΪ8��1�������� ������MLP�����ͬ
	cout << "��ǩ����Ϊ��" << endl;
	cout << labelsMat << endl;

	// ������ͼ��ȡ����һ�η���trainMat��ѵ��������
	Mat trainMat(12, smpW * smpH, CV_32FC1);
	//�������д�ѵ��ͼ��
	for (int i = 0; i < 3; i++)
	{
		for (int j = 1; j < 5; j++)
		{
			//����ͼ��
			string path = format("charData\\%d%d.png", i, j);
			cout << path << endl;
			Mat img = imread(path, 0);
			if (img.empty())
			{
				break;
			}
			//Canny(img, img, 50, 100);

			//ÿ�������ͼ����trainMat�е�����
			int index1 = i * 4 + j - 1;
			float* trainData = trainMat.ptr<float>(index1);
			//��imread������ͼ����д�������trainMat��
			//��trainData��Ϊ����
			for (int h = 0; h < smpH; h++)
			{
				uchar * imgData = img.ptr<uchar>(h);
				for (int w = 0; w < smpW; w++)
				{
					//ͼ���ÿ��ת����ΪMat��һ���е���һ����λ��
					int index2 = h*smpW + w;
					trainData[index2] = imgData[w];
				}
			}
		}
	}
	
	// BP ģ�ʹ����Ͳ�������
	Ptr<ANN_MLP> bp = ANN_MLP::create();
	
	// ���岢���ò�Ĵ�С
	// TODO �������������������4������
	Mat layerSizes = (Mat_<int>(1, 3) << smpW * smpH, 6, 3);// ����Ϊͼ���С
													  // �м����ز㣬Ĭ��Ϊ6
													 // ������м�������ü����ڵ㣬Ĭ����1��
	bp->setLayerSizes(layerSizes);
	
	// �����������
	bp->setTrainMethod(ANN_MLP::BACKPROP, 0.1, 0.1);  // ���򴫲���ѵ������
												// ����bp_dw_scale��bp_moment_scale��������
												// �ο����� https://blog.csdn.net/xiaowei_cqu/article/details/9027617
	bp->setActivationFunction(ANN_MLP::SIGMOID_SYM);  //��sigmoid����Ϊ�����
	bp->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 10000, /*FLT_EPSILON*/1e-6));

	// ����ѵ���õ����������
	bool trained = bp->train(trainMat, ROW_SAMPLE, labelsMat);  //ANN_MLP��̳���StatModelģ�ͣ�����train()ͨ��������������
														  // bool train( InputArray samples, int layout, InputArray responses )
														 // ����labelMat���к��зֱ������������ά����ͬ
	if (trained) 
	{
		cout << "save bp_param.xml" << endl;
		bp->save("bp_param.xml");
	}


	//����ѵ���õ�������
	cout << "load bp_param.xml file ..." << endl;
	bp = ANN_MLP::load<ANN_MLP>("bp_param.xml");   //����ѵ���õ�ģ��
	cout << "load bp_param.xml ok!" << endl;

	//����������ݣ�����������ѵ��������ͬ
	for (int i = 0; i < 3; i++)
	{
		for (int j = 5; j < 6; j++)  // ��05 ��15����ͼ�����Ԥ��
		{
			//��Ϊֻ�ǲ���һ��ͼ��testMat��һ��Ԫ��Ϊ�����Ե�ͼ������
			//������԰������index1����һ������������ֱ����count++����
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
			//����ÿ����������ݣ�ʹ�÷�������һ��Ԥ��
			Mat predictionMat(1,3,CV_32FC1);
			cout << "MLP predict is" << endl;
			float response = bp->predict(testMat, predictionMat);  // �������⣬������ʾʶ����
											// �÷���λ��statModelͳ��ģ���У��̳���Algorithm��
										    // testMat����ȡ����������Ϊ���룻responseΪһ������ѡ����������1*nά�ȣ�nΪ���flag��һ����ѡ��־λ
											// ����������������ʾʲô����ʾ����ͼ����Ŀ��ĸ���
											// ����ֵ��һ�����õ�floatֵ������
		//	float response = responseMat.ptr<float>(0)[0]; // Mat.ptrֱ�ӵõ�һ�е�ָ�룬����ȡ����һ�еĵ�һ������error��Ҫ��û��	
		//	cout << "Ԥ�⺯������ֵresponse: " << response << "\t<0Ϊ�ɹ�������ʧ��!>" << endl;
			float * data = predictionMat.ptr<float>(0); //data[0]ȡ����һ�е�һ����
			cout << "����ʶ���ͼ���ǣ�" << "��"<<i<<"��"<<"��" << j << "��" << endl;
			cout << "Ԥ�����"<< predictionMat << endl;
			//cout << "Ԥ����� data[0]:"<< data[0] <<"\n"<< endl;
			
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
				case 0: cout << "Ԥ����Ϊ��" << "��0��," << "����Ϊ" << max << endl; break;
				case 1: cout << "Ԥ����Ϊ��" << "��1��," << "����Ϊ" << max << endl;  break;
				case 2: cout << "Ԥ����Ϊ��" << "��2��," << "����Ϊ" << max << endl;  break;
				default: break;
				}
			}
			else
			{
				cout << "error δ��⵽�κ����" << endl;
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
// GPS���ݶ�ȡ�����㷽λ����

// Ŀ����
	//ѵ��
	// ��������ͼ�񣬲�������ͼ��resizeΪ�ƶ���С������
	imgResize();

	// ��ȡHOG����������HOG����ͼ������ݶ�ֱ��ͼ
	// OK

	// ����HOGͼ�����ѵ��
	// OK

	// ��resize���ͼ������SVM��MLP�з���ѵ��������������
	// ����������
	SVM_TEST();  // �������������ķ��࣬��ȡROI��������ANN��
	ANN_TEST();  // ����̬�ķ��࣬�����ķ��࣬���桢��45�㡢���桢����

  //���
	// �Ի�����ʽ��ȡ��������ͼ��
	// TODOhttps://docs.opencv.org/3.1.0/d0/dce/classcv_1_1ml_1_1ANN__MLP.png

	// ��ȡͼ���������뵽Ŀ����������з��࣬ʶ��Ŀ�꣬�õ��걸��ROI����
	// TODO

	// ��ROI�����HOG����ͼ���뵽��̬���������з��࣬�õ���̬������
	// TODO

	// ���ݷ����������������˻����ɿز�������
	// ����������ģ�����
	system("pause");
	return 0;
}

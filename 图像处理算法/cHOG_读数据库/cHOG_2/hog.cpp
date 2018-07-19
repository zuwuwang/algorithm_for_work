
/*****************************************
Copyright (c) 2015 Jingshuang Hu
@filename:demo.cpp
@datetime:2015.08.06
@author:HJS
@e-mail:eleftheria@163.com
@blog:http://blog.csdn.net/hujingshuang
*****************************************/


/*
* �ο����� https://blog.csdn.net/hujingshuang/article/details/47337707/
*/

/******************
***  �������ӣ� https://blog.csdn.net/coming_is_winter/article/details/72850511
******************/

/**
* hog����������ͼ����׼�� ��ô��ʾ�����������hog�����أ�
* �������ҵ�֪ʶˮƽ��⣬ͼ����׼�е��ѣ���Ϊ��������ͼ���һ��ͳ������
* ֻ����������ͼ���Ƿ�Ϊһ��ͼ����������������ˣ�
* ����sift����������⣬�������������Ҳû����ô��˵��.�Լ�����ʶ����һ���ԡ�
*/
#include <iostream>
#include <cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

/** 
	debugģʽ�±���
const string root = format("images");
const string srcImg = format("srcImg");
const string gammaImg = format("gammaImg");
const string gradientImg = format("gradientImg");
****/

const int classNum = 3;
const int imgNum = 18;

Mat getHOG(Mat srcImg)
{
	Mat gradientImgImg;

	return gradientImgImg;
}
 
int main()
{
	int imgCount = 0;
	for (int i = 0; i < classNum; i++)
	{
		for (int j = 1; j < imgNum; j++)
		{
			string srcImgPath = format("images\\srcImg\\%d%d.jpg", i, j);
			string gradientImgPath = format("images\\gradientImg\\%d%d.jpg",i,j);
			Mat srcImg = imread(srcImgPath, CV_LOAD_IMAGE_GRAYSCALE);//�ԻҶ�ͼ��ʽ����
			
			imgCount++;
			if (srcImg.empty())
			{
				break;
			}
		    // do somethong here
			//Mat picture = imread("1.jpg", 0);//�Ҷ�
			Mat picture = srcImg;
			Mat img;
			picture.convertTo(img, CV_32F);	//ת���ɸ���
			
			sqrt(img, img);					//gammaУ��

			// ����ʹͼ��Աȶ���ǿ��ʹ�ø�����������
			// do something here
			normalize(img, img, 0, 255, NORM_MINMAX, CV_32F);//��һ��[0,255]������

			Mat gradientImg = Mat::zeros(img.rows, img.cols, CV_32F);//�ݶ�
			Mat theta = Mat::zeros(img.rows, img.cols, CV_32F);//�Ƕ�

			for (int i = 1; i < img.rows - 1; i++)
			{
				for (int j = 1; j < img.cols - 1; j++)
				{
					float Gx, Gy;

					Gx = img.at<float>(i, j + 1) - img.at<float>(i, j - 1);
					Gy = img.at<float>(i + 1, j) - img.at<float>(i - 1, j);

					gradientImg.at<float>(i, j) = sqrt(Gx * Gx + Gy * Gy);//�ݶ�ģֵ
					theta.at<float>(i, j) = float(atan2(Gy, Gx) * 180 / CV_PI);//�ݶȷ���[-180�㣬180��]
				}
			}

			normalize(gradientImg, gradientImg, 0, 255, NORM_MINMAX, CV_8UC1);//��һ��[0,255] �޷�������
			normalize(img, img, 0, 255, NORM_MINMAX, CV_8UC1);

			imshow("�Ҷ�ͼ", picture);
			imshow("GammaУ����", img);
			imshow("�ݶ�ͼ", gradientImg);
			imwrite(gradientImgPath, gradientImg);
			cout << imgCount << endl;
 			waitKey();
		}
		
	}
	
	waitKey(0);
	return 0;
}



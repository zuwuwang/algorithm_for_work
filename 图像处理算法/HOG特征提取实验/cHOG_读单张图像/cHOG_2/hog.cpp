
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

int main()
{
	Mat picture = imread("01.jpg", 0);//�Ҷ�
	Mat img;

	picture.convertTo(img, CV_32F);	//ת���ɸ���
	sqrt(img, img);					//gammaУ��
	normalize(img, img, 0, 255, NORM_MINMAX, CV_32F);//��һ��[0,255]������

	Mat gradient = Mat::zeros(img.rows, img.cols, CV_32F);//�ݶ�
	Mat theta = Mat::zeros(img.rows, img.cols, CV_32F);//�Ƕ�

	for (int i = 1; i < img.rows - 1; i++)
	{
		for (int j = 1; j < img.cols - 1; j++)
		{
			float Gx, Gy;

			Gx = img.at<float>(i, j + 1) - img.at<float>(i, j - 1);
			Gy = img.at<float>(i + 1, j) - img.at<float>(i - 1, j);

			gradient.at<float>(i, j) = sqrt(Gx * Gx + Gy * Gy);//�ݶ�ģֵ
			theta.at<float>(i, j) = float(atan2(Gy, Gx) * 180 / CV_PI);//�ݶȷ���[-180�㣬180��]
		}
	}

	normalize(gradient, gradient, 0, 255, NORM_MINMAX, CV_8UC1);//��һ��[0,255] �޷�������
	normalize(img, img, 0, 255, NORM_MINMAX, CV_8UC1);

	imshow("ԭͼ", picture);
	imshow("GammaУ��", img);
	imshow("�ݶ�ͼ", gradient);
	waitKey();

	return 0;
}



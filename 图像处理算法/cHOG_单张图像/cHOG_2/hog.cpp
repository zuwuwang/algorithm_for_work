
/*****************************************
Copyright (c) 2015 Jingshuang Hu
@filename:demo.cpp
@datetime:2015.08.06
@author:HJS
@e-mail:eleftheria@163.com
@blog:http://blog.csdn.net/hujingshuang
*****************************************/


/*
* 参考链接 https://blog.csdn.net/hujingshuang/article/details/47337707/
*/

/******************
***  测试链接： https://blog.csdn.net/coming_is_winter/article/details/72850511
******************/

/**
* hog特征能用于图像配准吗？ 怎么表示单个特征点的hog特征呢？
* 以现在我的知识水平理解，图像配准有点难，因为他是整个图像的一个统计量，
* 只是用来区分图像是否为一类图像，诸如包不包含行人，
* 不像sift那样特征检测，单个特征点好像也没有这么个说法.自己简单认识，不一定对。
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
	Mat picture = imread("01.jpg", 0);//灰度
	Mat img;

	picture.convertTo(img, CV_32F);	//转换成浮点
	sqrt(img, img);					//gamma校正
	normalize(img, img, 0, 255, NORM_MINMAX, CV_32F);//归一化[0,255]浮点数

	Mat gradient = Mat::zeros(img.rows, img.cols, CV_32F);//梯度
	Mat theta = Mat::zeros(img.rows, img.cols, CV_32F);//角度

	for (int i = 1; i < img.rows - 1; i++)
	{
		for (int j = 1; j < img.cols - 1; j++)
		{
			float Gx, Gy;

			Gx = img.at<float>(i, j + 1) - img.at<float>(i, j - 1);
			Gy = img.at<float>(i + 1, j) - img.at<float>(i - 1, j);

			gradient.at<float>(i, j) = sqrt(Gx * Gx + Gy * Gy);//梯度模值
			theta.at<float>(i, j) = float(atan2(Gy, Gx) * 180 / CV_PI);//梯度方向[-180°，180°]
		}
	}

	normalize(gradient, gradient, 0, 255, NORM_MINMAX, CV_8UC1);//归一化[0,255] 无符号整型
	normalize(img, img, 0, 255, NORM_MINMAX, CV_8UC1);

	imshow("原图", picture);
	imshow("Gamma校正", img);
	imshow("梯度图", gradient);
	waitKey();

	return 0;
}



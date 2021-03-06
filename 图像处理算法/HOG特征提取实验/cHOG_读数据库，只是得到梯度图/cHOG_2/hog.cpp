
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

/** 
	debug模式下报错
const string root = format("images");
const string srcImg = format("srcImg");
const string gammaImg = format("gammaImg");
const string gradientImg = format("gradientImg");
****/

const int classNum = 3;
const int imgNum = 50;// jpg = 18,png = 50

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
		for (int j = 1; j <= imgNum; j++)
		{
			// jpg or png?
			string srcImgPath = format("images\\srcImg\\charData\\%d%d.png", i, j);
			string gradientImgPath = format("images\\gradientImg\\charData\\%d%d.png",i,j);
			Mat srcImg = imread(srcImgPath, CV_LOAD_IMAGE_GRAYSCALE);//以灰度图形式读入
			
			imgCount++;
			if (srcImg.empty())
			{
				break;
			}
		    // do somethong here
			//Mat picture = imread("1.jpg", 0);//灰度
			Mat picture = srcImg;
			Mat img;
			picture.convertTo(img, CV_32F);	//转换成浮点
			
			sqrt(img, img);					//gamma校正

			// 考虑使图像对比度增强，使得杆塔更加明显
			// do something here
			normalize(img, img, 0, 255, NORM_MINMAX, CV_32F);//归一化[0,255]浮点数

			Mat gradientImg = Mat::zeros(img.rows, img.cols, CV_32F);//梯度
			Mat theta = Mat::zeros(img.rows, img.cols, CV_32F);//角度
			
			Mat Gx = Mat::zeros(img.rows, img.cols, CV_32F);//梯度
			Mat Gy = Mat::zeros(img.rows, img.cols, CV_32F);

			Mat gradscal_x = Mat::zeros(img.rows, img.cols, CV_32F);//水平梯度分量
			Mat gradscal_y = Mat::zeros(img.rows, img.cols, CV_32F);//竖直梯度分量

			Mat kernel_x(1, 3, CV_32F, Scalar(-1, 0, 1));//[-1,0,1]梯度算子
			Mat kernel_y(1, 3, CV_32F, Scalar(1, 0, -1));
			
			//图像卷积操作，即滤波操作，在opencv中已经封装成为了函数，注意学习其底层实现方式
			filter2D(img, gradscal_x, img.depth(), kernel_x);
			filter2D(img, gradscal_y, img.depth(), kernel_y);
			for (int i = 1; i < img.rows - 1; i++)
			{
				for (int j = 1; j < img.cols - 1; j++)
				{
					/*
					float Gx, Gy;
					Gx = img.at<float>(i, j + 1) - img.at<float>(i, j - 1);
					Gy = img.at<float>(i + 1, j) - img.at<float>(i - 1, j);
					gradientImg.at<float>(i, j) = sqrt(Gx * Gx + Gy * Gy);//梯度模值
					theta.at<float>(i, j) = float(atan2(Gy, Gx) * 180 / CV_PI);//梯度方向[-180°，180°]
					*/

					Gx.at<float>(i, j) = gradscal_x.at<float>(i, j + 1) - gradscal_x.at<float>(i, j - 1);
					Gy.at<float>(i, j) = gradscal_y.at<float>(i + 1, j) - gradscal_y.at<float>(i - 1, j);
					gradientImg.at<float>(i, j) = sqrt(Gx.at<float>(i, j)*Gx.at<float>(i, j) + Gy.at<float>(i, j)*Gy.at<float>(i, j));
					theta.at<float>(i, j) = float(atan2(Gy.at<float>(i, j), Gx.at<float>(i, j)) * 180 / CV_PI);
				}
			}
			// 归一化
			normalize(gradientImg, gradientImg, 0, 255, NORM_MINMAX, CV_8UC1);//归一化[0,255] 无符号整型
			normalize(img, img, 0, 255, NORM_MINMAX, CV_8UC1);
			// 提取HOG特征描述

			// 显示
			imshow("灰度图", picture);
			imshow("Gamma校正后", img);
			imshow("梯度图", gradientImg);
			imwrite(gradientImgPath, gradientImg);
			cout << imgCount << endl;
 			// waitKey();
			// 这里只是得到了梯度图还没有统计梯度方向和向量化
		}
		
	}
	
	waitKey(0);
	return 0;
}



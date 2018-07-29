#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

#define slide_W 48
#define slide_H 48
#define slide_stride 24

int main()
{
	string path = format("1.jpg");
	Mat ROI;
	Mat srcImg = imread(path, CV_LOAD_IMAGE_ANYDEPTH);
	if (srcImg.empty())
		return -1;
	imshow("srcImg", srcImg);
	int img_H = srcImg.rows;
	int img_W = srcImg.cols;
	int x = 0, y = 0;
	int windowCount = 0;
	vector<Point> pointRemembered;  //存放识别结果为目标的各个点
	Point pointA, pointB, pointC, pointD; //boundingbox 的四个坐标点
	for (y = 0; y < img_H - slide_H; y = y + slide_stride)
	{
		cout << y;
		for (x = 0; x < img_W - slide_W; x = x + slide_stride)
		{
			Rect slidingWindow(x, y, slide_W, slide_H);
			cout <<"滑窗的面积为："<< slidingWindow.area() << endl;
			cout << "各个滑窗顶点坐标为:"<<"(" << x << "," << y << ")" << endl;
			ROI = srcImg(Rect(x, y,  slide_W, slide_H));
			++windowCount;
			string newName = format("slideWindow\\%d.jpg", windowCount);
			namedWindow("ROI", WINDOW_KEEPRATIO);
			imshow("ROI", ROI);
			imwrite(newName, ROI);

			// 画框 boundingbox
			pointA.x = x;
			pointA.y = y;
			pointD.x = x + slide_W;
			pointD.y = y + slide_H;
			pointB.x = x + slide_W;
			pointB.y = y;
			pointC.x = x;
			pointC.y = y + slide_H;

			line(srcImg, pointA, pointB, Scalar(255, 255, 255));
			circle(srcImg,pointA,4,Scalar(0,0,0));
			circle(srcImg, pointB, 4, Scalar(0, 0, 0));
			imshow("lineSrcImg", srcImg);
			//cvWaitKey(0);

			line(srcImg, pointA, pointC, Scalar(255, 255, 255));
			circle(srcImg, pointC, 4, Scalar(0, 0, 0));
			imshow("lineSrcImg", srcImg);
			//cvWaitKey(0);

			line(srcImg, pointC, pointD, Scalar(255, 255, 255));
			imshow("lineSrcImg", srcImg);
			//cvWaitKey(0);

			line(srcImg, pointD, pointB, Scalar(255, 255, 255));
			imshow("lineSrcImg", srcImg);
			//cvWaitKey(0);

			//// 画斜线
			//line(srcImg, pointA, pointD, Scalar(255, 255, 255));
			//imshow("lineSrcImg", srcImg);

			// TODO HERE
			bool someFlag =false;
			if (someFlag)
			{
				Point point;
				pointRemembered.push_back(point);
			}

		}
	}

	waitKey(0);
	system("pause");
	return 0;
}
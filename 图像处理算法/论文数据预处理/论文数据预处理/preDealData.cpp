#include <iostream>
#include <string>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>

using namespace std;
using namespace cv;

int imgResize(string srcPath,string savePath,int imgNum)
{
	for (int i = 1; i <= imgNum; i++)
	{
		string readPath = srcPath + format("%d",i) + ".jpg";
		string writePath = savePath + format("%d", i) + ".jpg";
		Mat img = imread(readPath, CV_LOAD_IMAGE_COLOR);
		if (img.empty())
		{
			return 0;
		}
		else
		{
			// 将训练样本resize为64*128大小
			// 每个ROI框的大小也设置为 64*128
			resize(img, img, Size(64, 128));
			cout << "当前正在保存的图像为：" << writePath << endl;
			imwrite(writePath, img);
			for (int i = 0; i < 1000; i++);
		}
	}
	return 0;
}
void main()
{
	int roi_w = 64;
	int roi_h = 128;
	int j = 1;

	// 处理得到负样本
	cout << "【1】对负样本进行resize" << endl;
	/*for(int i = 1; i <= 2; i=i++)
	{
		string path = format("G:\\学术研究\\数据集\\原始\\负样本\\%d.jpg",i);
		Mat srcImg = imread(path, CV_LOAD_IMAGE_COLOR);
		if (srcImg.empty())
			return;
		for (int y = 0; y < srcImg.rows - roi_h; y = y + roi_h)
		{
			for (int x = 0; x < srcImg.cols - roi_w; x = x + roi_w)
			{
				Mat roi = srcImg(Rect(x, y, roi_w, roi_h));
				string roiPath = format("G:\\学术研究\\数据集\\原始\\负样本\\cropROI\\%d.jpg", j);
				imwrite(roiPath, roi);
				j++;
				for (int i = 0; i < 1000; i++);
			}
		}
	}*/
	
	// 将负样本resize
	// 上面的操作就是将图像切分成小的ROI块作为负样本

	// 将正样本resize
	// 侧面塔身、塔顶、正面
	cout << "【2】开始resize杆塔侧身，无倾斜角..." << endl;
	imgResize("G:\\学术研究\\数据集\\分类好的\\训练数据集\\正样本【resize到64 128】450张\\杆塔1-干字塔\\侧面塔身\\无倾斜角\\",
		"G:\\学术研究\\数据集\\分类好的\\训练数据集\\正样本【resize到64 128】450张\\杆塔1-干字塔\\侧面塔身\\无倾斜角\\resized\\",
		188);
	
	cout << "【2】开始resize杆塔侧身，有倾斜角..." << endl;
	imgResize("G:\\学术研究\\数据集\\分类好的\\训练数据集\\正样本【resize到64 128】450张\\杆塔1-干字塔\\侧面塔身\\有倾斜角\\",
		"G:\\学术研究\\数据集\\分类好的\\训练数据集\\正样本【resize到64 128】450张\\杆塔1-干字塔\\侧面塔身\\有倾斜角\\resized\\",
		101);
	
	cout << "【3】开始resize杆塔顶..." << endl;
	imgResize("G:\\学术研究\\数据集\\分类好的\\训练数据集\\正样本【resize到64 128】450张\\杆塔1-干字塔\\塔顶\\",
		"G:\\学术研究\\数据集\\分类好的\\训练数据集\\正样本【resize到64 128】450张\\杆塔1-干字塔\\塔顶\\resized\\",
		124);
	
	cout << "【4】开始resize杆塔正面..." << endl;
	imgResize("G:\\学术研究\\数据集\\分类好的\\训练数据集\\正样本【resize到64 128】450张\\杆塔1-干字塔\\正面\\",
		"G:\\学术研究\\数据集\\分类好的\\训练数据集\\正样本【resize到64 128】450张\\杆塔1-干字塔\\正面\\resized\\",
		189);

	return;
}
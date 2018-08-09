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
			// ��ѵ������resizeΪ64*128��С
			// ÿ��ROI��Ĵ�СҲ����Ϊ 64*128
			resize(img, img, Size(64, 128));
			cout << "��ǰ���ڱ����ͼ��Ϊ��" << writePath << endl;
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

	// ����õ�������
	cout << "��1���Ը���������resize" << endl;
	/*for(int i = 1; i <= 2; i=i++)
	{
		string path = format("G:\\ѧ���о�\\���ݼ�\\ԭʼ\\������\\%d.jpg",i);
		Mat srcImg = imread(path, CV_LOAD_IMAGE_COLOR);
		if (srcImg.empty())
			return;
		for (int y = 0; y < srcImg.rows - roi_h; y = y + roi_h)
		{
			for (int x = 0; x < srcImg.cols - roi_w; x = x + roi_w)
			{
				Mat roi = srcImg(Rect(x, y, roi_w, roi_h));
				string roiPath = format("G:\\ѧ���о�\\���ݼ�\\ԭʼ\\������\\cropROI\\%d.jpg", j);
				imwrite(roiPath, roi);
				j++;
				for (int i = 0; i < 1000; i++);
			}
		}
	}*/
	
	// ��������resize
	// ����Ĳ������ǽ�ͼ���зֳ�С��ROI����Ϊ������

	// ��������resize
	// ������������������
	cout << "��2����ʼresize������������б��..." << endl;
	imgResize("G:\\ѧ���о�\\���ݼ�\\����õ�\\ѵ�����ݼ�\\��������resize��64 128��450��\\����1-������\\��������\\����б��\\",
		"G:\\ѧ���о�\\���ݼ�\\����õ�\\ѵ�����ݼ�\\��������resize��64 128��450��\\����1-������\\��������\\����б��\\resized\\",
		188);
	
	cout << "��2����ʼresize������������б��..." << endl;
	imgResize("G:\\ѧ���о�\\���ݼ�\\����õ�\\ѵ�����ݼ�\\��������resize��64 128��450��\\����1-������\\��������\\����б��\\",
		"G:\\ѧ���о�\\���ݼ�\\����õ�\\ѵ�����ݼ�\\��������resize��64 128��450��\\����1-������\\��������\\����б��\\resized\\",
		101);
	
	cout << "��3����ʼresize������..." << endl;
	imgResize("G:\\ѧ���о�\\���ݼ�\\����õ�\\ѵ�����ݼ�\\��������resize��64 128��450��\\����1-������\\����\\",
		"G:\\ѧ���о�\\���ݼ�\\����õ�\\ѵ�����ݼ�\\��������resize��64 128��450��\\����1-������\\����\\resized\\",
		124);
	
	cout << "��4����ʼresize��������..." << endl;
	imgResize("G:\\ѧ���о�\\���ݼ�\\����õ�\\ѵ�����ݼ�\\��������resize��64 128��450��\\����1-������\\����\\",
		"G:\\ѧ���о�\\���ݼ�\\����õ�\\ѵ�����ݼ�\\��������resize��64 128��450��\\����1-������\\����\\resized\\",
		189);

	return;
}
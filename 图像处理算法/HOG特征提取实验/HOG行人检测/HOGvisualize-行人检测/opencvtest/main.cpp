#include "opencvtest.h"

using namespace std;
using namespace cv;

// HOGDescriptor visual_imagealizer
// adapted for arbitrary size of feature sets and training images
Mat get_hogdescriptor_visual_image(Mat& origImg,
	vector<float>& descriptorValues,//hog��������
	Size winSize,//ͼƬ���ڴ�С
	Size cellSize,             
	int scaleFactor,//���ű���ͼ��ı���
	double viz_factor)//����hog�������߳�����
{   
	Mat visual_image;//�����ӻ���ͼ���С
	resize(origImg, visual_image, Size(origImg.cols*scaleFactor, origImg.rows*scaleFactor));

	int gradientBinSize = 9;
	// dividing 180�� into 9 bins, how large (in rad) is one bin?
	float radRangeForOneBin = 3.14/(float)gradientBinSize; //pi=3.14��Ӧ180��

	// prepare data structure: 9 orientation / gradient strenghts for each cell
	int cells_in_x_dir = winSize.width / cellSize.width;//x�����ϵ�cell����
	int cells_in_y_dir = winSize.height / cellSize.height;//y�����ϵ�cell����
	int totalnrofcells = cells_in_x_dir * cells_in_y_dir;//cell���ܸ���
	//ע��˴���ά����Ķ����ʽ
	//int ***b;
	//int a[2][3][4];
	//int (*b)[3][4] = a;
	//gradientStrengths[cells_in_y_dir][cells_in_x_dir][9]
	float*** gradientStrengths = new float**[cells_in_y_dir];
	int** cellUpdateCounter   = new int*[cells_in_y_dir];
	for (int y=0; y<cells_in_y_dir; y++)
	{
		gradientStrengths[y] = new float*[cells_in_x_dir];
		cellUpdateCounter[y] = new int[cells_in_x_dir];
		for (int x=0; x<cells_in_x_dir; x++)
		{
			gradientStrengths[y][x] = new float[gradientBinSize];
			cellUpdateCounter[y][x] = 0;

			for (int bin=0; bin<gradientBinSize; bin++)
				gradientStrengths[y][x][bin] = 0.0;//��ÿ��cell��9��bin��Ӧ���ݶ�ǿ�ȶ���ʼ��Ϊ0
		}
	}

	// nr of blocks = nr of cells - 1
	// since there is a new block on each cell (overlapping blocks!) but the last one
	//�൱��blockstride = (8,8)
	int blocks_in_x_dir = cells_in_x_dir - 1;
	int blocks_in_y_dir = cells_in_y_dir - 1;

	// compute gradient strengths per cell
	int descriptorDataIdx = 0;
	int cellx = 0;
	int celly = 0;

	for (int blockx=0; blockx<blocks_in_x_dir; blockx++)
	{
		for (int blocky=0; blocky<blocks_in_y_dir; blocky++)            
		{
			// 4 cells per block ...
			for (int cellNr=0; cellNr<4; cellNr++)
			{
				// compute corresponding cell nr
				int cellx = blockx;
				int celly = blocky;
				if (cellNr==1) celly++;
				if (cellNr==2) cellx++;
				if (cellNr==3)
				{
					cellx++;
					celly++;
				}

				for (int bin=0; bin<gradientBinSize; bin++)
				{
					float gradientStrength = descriptorValues[ descriptorDataIdx ];
					descriptorDataIdx++;

					gradientStrengths[celly][cellx][bin] += gradientStrength;//��ΪC�ǰ��д洢

				} // for (all bins)


				// note: overlapping blocks lead to multiple updates of this sum!
				// we therefore keep track how often a cell was updated,
				// to compute average gradient strengths
				cellUpdateCounter[celly][cellx]++;//����block֮�����ص�������Ҫ��¼��Щcell����μ�����

			} // for (all cells)


		} // for (all block x pos)
	} // for (all block y pos)


	// compute average gradient strengths
	for (int celly=0; celly<cells_in_y_dir; celly++)
	{
		for (int cellx=0; cellx<cells_in_x_dir; cellx++)
		{

			float NrUpdatesForThisCell = (float)cellUpdateCounter[celly][cellx];

			// compute average gradient strenghts for each gradient bin direction
			for (int bin=0; bin<gradientBinSize; bin++)
			{
				gradientStrengths[celly][cellx][bin] /= NrUpdatesForThisCell;
			}
		}
	}

	
	cout << "winSize = " << winSize << endl;
	cout << "cellSize = " << cellSize << endl;
	cout << "blockSize = " << cellSize*2<< endl;
	cout << "blockNum = " << blocks_in_x_dir<<"��"<<blocks_in_y_dir << endl;
	cout << "descriptorDataIdx = " << descriptorDataIdx << endl;

	// draw cells
	for (int celly=0; celly<cells_in_y_dir; celly++)
	{
		for (int cellx=0; cellx<cells_in_x_dir; cellx++)
		{
			int drawX = cellx * cellSize.width;
			int drawY = celly * cellSize.height;

			int mx = drawX + cellSize.width/2;
			int my = drawY + cellSize.height/2;

			rectangle(visual_image,
				Point(drawX*scaleFactor,drawY*scaleFactor),
				Point((drawX+cellSize.width)*scaleFactor,
				(drawY+cellSize.height)*scaleFactor),
				CV_RGB(0,0,0),//cell���ߵ���ɫ
				1);

			// draw in each cell all 9 gradient strengths
			for (int bin=0; bin<gradientBinSize; bin++)
			{
				float currentGradStrength = gradientStrengths[celly][cellx][bin];

				// no line to draw?
				if (currentGradStrength==0)
					continue;

				float currRad = bin * radRangeForOneBin + radRangeForOneBin/2;//ȡÿ��bin����м�ֵ����10��,30��,...,170��.

				float dirVecX = cos( currRad );
				float dirVecY = sin( currRad );
				float maxVecLen = cellSize.width/2;
				float scale = viz_factor; // just a visual_imagealization scale,
				// to see the lines better

				// compute line coordinates
				float x1 = mx - dirVecX * currentGradStrength * maxVecLen * scale;
				float y1 = my - dirVecY * currentGradStrength * maxVecLen * scale;
				float x2 = mx + dirVecX * currentGradStrength * maxVecLen * scale;
				float y2 = my + dirVecY * currentGradStrength * maxVecLen * scale;

				// draw gradient visual_imagealization
				line(visual_image,
					Point(x1*scaleFactor,y1*scaleFactor),
					Point(x2*scaleFactor,y2*scaleFactor),
					CV_RGB(255,255,255),//HOG���ӻ���cell����ɫ
					1);

			} // for (all bins)

		} // for (cellx)
	} // for (celly)


	// don't forget to free memory allocated by helper data structures!
	for (int y=0; y<cells_in_y_dir; y++)
	{
		for (int x=0; x<cells_in_x_dir; x++)
		{
			delete[] gradientStrengths[y][x];            
		}
		delete[] gradientStrengths[y];
		delete[] cellUpdateCounter[y];
	}
	delete[] gradientStrengths;
	delete[] cellUpdateCounter;

	return visual_image;//�������յ�HOG���ӻ�ͼ��

}


int main()
{
	
	HOGDescriptor hog;//ʹ�õ���Ĭ�ϵ�hog����
	/*
	HOGDescriptor(Size win_size=Size(64, 128), Size block_size=Size(16, 16), Size block_stride=Size(8, 8), 
	Size cell_size=Size(8, 8), int nbins=9, double win_sigma=DEFAULT_WIN_SIGMA(DEFAULT_WIN_SIGMA=-1), 
	double threshold_L2hys=0.2, bool gamma_correction=true, int nlevels=DEFAULT_NLEVELS)

	Parameters:	
	win_size �C Detection window size. Align to block size and block stride.
	block_size �C Block size in pixels. Align to cell size. Only (16,16) is supported for now.
	block_stride �C Block stride. It must be a multiple of cell size.
	cell_size �C Cell size. Only (8, 8) is supported for now.
	nbins �C Number of bins. Only 9 bins per cell are supported for now.
	win_sigma �C Gaussian smoothing window parameter.
	threshold_L2hys �C L2-Hys normalization method shrinkage.
	gamma_correction �C Flag to specify whether the gamma correction preprocessing is required or not.
	nlevels �C Maximum number of detection window increases.
	*/
	//����128*80��ͼƬ��blockstride = 8,15*9��block��2*2*9*15*9 = 4860

	int width = 128;
	int height = 128;
	hog.winSize=Size(width,height);
	//hog.cellSize = Size(4, 4);
	//hog.blockStride(4);
	//hog.blockSize(2);
	vector<float> des;//HOG��������
	Mat src = imread("1.jpg");
	src.resize(128, 128);
	Mat dst ;
	resize(src,dst,Size(width,height));//�淶ͼ��ߴ�
	imshow("src",src);
	hog.compute(dst,des);//����hog����
	Mat background = Mat::zeros(Size(width,height),CV_8UC1);//���ú�ɫ����ͼ����ΪҪ�ð�ɫ����hog����

	Mat d = get_hogdescriptor_visual_image(background,des,hog.winSize,hog.cellSize,3,2.5);
	imshow("dst",d);
	imwrite("hogvisualize.jpg",d);
	waitKey();

	 return 0;
}

#include<cv.h>
#include<highgui.h>
#include<math.h>
#include<opencv.hpp>
using namespace cv;
using namespace std;
IplImage * GRBtoGrey(IplImage* img)//RGB图像转为灰度图像
{
	IplImage* dest = cvCreateImage(cvSize(img->width, img->height), img->depth, 1);


	uchar* data = (uchar*)img->imageData;
	int b, g, r;
	for (int i = 0; i<img->height; i++){
		for (int j = 0; j<img->width; j++){
			b = data[i*img->widthStep + j*img->nChannels + 0];
			g = data[i*img->widthStep + j*img->nChannels + 1];
			r = data[i*img->widthStep + j*img->nChannels + 2];
			((uchar*)(dest->imageData + i*dest->widthStep))[j] = 0.3*r + 0.59*g + 0.11*b;
		}
	}
	return dest;
}


IplImage* nomolize(IplImage *img)//必须是灰度图
{
	uchar* gM = (uchar*)img->imageData;
	int max = 0, min = 255;
	for (int i = 0; i<img->height; i++){
		for (int j = 0; j<img->width; j++){
			int num = gM[i*img->widthStep + j*img->nChannels];
			if (num>max){
				max = num;
			}
			if (num<min){
				min = num;
			}
		}
	}
	for (int i = 0; i<img->height; i++){
		for (int j = 0; j<img->width; j++){
			int num = (int)(float)(gM[i*img->widthStep + j*img->nChannels] - min)*
				((float)(255) / (float)(max - min));
			if (num>255){
				num = 255;
			}
			((uchar*)(img->imageData + i*img->widthStep))[j] = num;
		}
	}
	return img;
}


IplImage *greygamma(IplImage *greyimage){
	IplImage *gammaimage = cvCreateImage(cvSize(greyimage->width, greyimage->height),
		greyimage->depth, greyimage->nChannels);


	uchar* data = (uchar*)greyimage->imageData;
	for (int i = 0; i<greyimage->height; i++){
		for (int j = 0; j<greyimage->width; j++){
			int gamma = data[i*greyimage->widthStep + j*greyimage->nChannels];
			((uchar*)(gammaimage->imageData + i*gammaimage->widthStep))[j] = (int)sqrt((float)gamma);
		}
	}
	return nomolize(gammaimage);
}


IplImage *gradientimg(IplImage *img){
	uchar * data = (uchar*)img->imageData;
	for (int i = 1; i<img->height - 1; i++){
		for (int j = 1; j<img->width - 1; j++){
			int Gx = data[i*img->widthStep + (j + 1)*img->nChannels] - data[i*img->widthStep + (j - 1)*img->nChannels];
			int Gy = data[(i + 1)*img->widthStep + j*img->nChannels] - data[(i - 1)*img->widthStep + j*img->nChannels];
			int gradient = (int)sqrt((float)(Gx*Gx + Gy*Gy));
			int theta = (int)(atan((float)Gy / (float)Gx) * 180 / 3.14);
			((uchar*)(img->imageData + i*img->widthStep))[j] = theta;
		}
	}
	return  nomolize(img);
}


int *CellFeature(IplImage *img, int w, int h){//8*8 cell
	int cell_w = 8;
	int f_n = 9;
	int *feature = new int[f_n];
	for (int i = 0; i<f_n; i++){
		f_n = 0;
	}
	for (int i = h; i<h + cell_w; i++){
		for (int j = w; j<w + cell_w; j++){
			feature[((uchar*)(img->imageData + i*img->widthStep))[j] / 40]++;
		}
	}
	return feature;
}


int *HogFeature(IplImage *img){


	int cell_w = 8;
	int cell_fn = 9;
	int cell_wn = img->width / cell_w;
	int cell_hn = img->height / cell_w;
	int *hog_f = new int[cell_wn*cell_hn*cell_fn];
	int m = 0;


	for (int i = 0; i<cell_hn; i++){
		for (int j = 0; j<cell_wn; j++){
			int *feature = CellFeature(img, j*cell_w, i);
			for (int k = 0; k<cell_fn; k++){
				hog_f[m++] = feature[k];
			}
		}
	}


	for (int i = 0; i<cell_wn*cell_hn*cell_fn; i++){
		if (i % 9 == 0){
			printf("\n");
		}
		if (i % (2 * 2 * 9) == 0){
			printf("-------------------------------------\n\n");
		}
		printf("%d ", hog_f[i]);
	}
	return hog_f;
}


int main()
{
	IplImage *img = cvLoadImage("Img2.jpg");
	if (img == NULL){
		printf("img is null");
		return 0;
	}
	cvNamedWindow("greyimage");
	IplImage* greyimg = GRBtoGrey(img);
	cvShowImage("greyimage",greyimg);
	cvWaitKey();
	cvNamedWindow("gammaimage");
	IplImage* gammaimg = greygamma(greyimg);
	cvShowImage("gammaimage",gammaimg);
	printf("gamma end\n");
	cvWaitKey();
	cvNamedWindow("gradientimage");
	IplImage* gradientimage = gradientimg(gammaimg);
	cvShowImage("gradientimage",gradientimage);
	printf("gradient end\n");
	cvWaitKey();
	int *hog_f = HogFeature(gradientimage);
	system("pause");
	return 1;
}
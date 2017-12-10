#include <iostream>
#include"PlateFind.h"
using namespace std;

int main() {
	IplImage * srcImg = cvLoadImage("bsx1.jpg", CV_LOAD_IMAGE_COLOR);
	//IplImage * srcImgGray = cvLoadImage("bsx1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	
	IplImage * srcImgGray = grayScale(srcImg);
	IplImage * img;
	vector<IplImage*> imgVector;
	vector<IplImage*> imgVectornew;
	vector<IplImage*> imgVectornewiiiii;
	img = FindPlate(srcImg);
	imgVector = FindCharacter(img);
	imgVectornew = resizeVectorImage(imgVector);
	showImgvector(imgVectornew);
	showcharacterFinal(imgVectornew);
	//////////////////////////////////////////////////////////////
	/*IplImage * img = cvLoadImage("Data/1.jpg");
	IplImage * imgResize = resize(img, 20, 30);
	cvShowImage("1", imgResize);
	CvMat * Matimg = convertIplimagetoCvMat(imgResize);*/
	/*cout << img->imageSize << endl;
	cout << imgResize->imageSize << endl;
	cvSaveImage("newimg.jpg", imgResize, 0);
	cvShowImage("2", imgResize);*/
	//showCvMatFloat(Matimg);
	///////////////////////////////////////////////////////////
	//loadImgData(4);
	cvWaitKey();
	//loadImgData(1);
	system("pause");
	return 0;
}
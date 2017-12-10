#pragma once
#include <iostream>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv\cxcore.h>
#include <vector>
#include <stdio.h>
#include <string>
using namespace std;

#ifndef _PLATEFIND_H_
#define _PLATEFIND_H_
//
	IplImage * grayScale(IplImage*src);
	IplImage * ImageRestoration(IplImage *src);	// tien xu ly anh goc
	IplImage * resizex2(IplImage*src);
	IplImage * resize(IplImage*src,int x,int y);
	IplImage* FindPlate(IplImage *src);	// tim va cat bien so
	int CountContour(IplImage*src);	// dem so vung co kha nang la ki tu
	vector <IplImage*> FindCharacter(IplImage*src);
	//void recognictionCharacter(vector<IplImage*>src);
	//void grayScale(IplImage* src);
	CvMat* convertIplimagetoCvMat(IplImage*src);
	vector<IplImage*> resizeVectorImage(vector<IplImage*>src);
	void showImgvector(vector<IplImage*>src);
	void showCvMatFloat(CvMat*src);
	void showIplimagedata(IplImage*src);
	void showcharacterFinal(vector<IplImage*>src);
	float sumMat(CvMat*src);
	IplImage* loadImgData(int x);
	void showCvMatFloatAbs(CvMat*src);
	int getDataClassifications(int x);
	//CvMat* convertMat(CvMat*src);
//
#endif // !_PLATEFIND_H_

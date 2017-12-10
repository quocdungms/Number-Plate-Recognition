#include "PlateFind.h"
#define S1  cvCreateStructuringElementEx(3, 1, 1, 0, CV_SHAPE_RECT, NULL)
#define S2  cvCreateStructuringElementEx(6, 1, 3, 0, CV_SHAPE_RECT, NULL)
const CvScalar RED = CV_RGB(255, 0, 0);
const CvScalar GREEN = CV_RGB(0, 255, 0);
const CvScalar BLUE = CV_RGB(0, 0, 255);
const CvScalar UNKNOW = CV_RGB(30, 125, 255);
#define SIZE_IMAGE_WIDTH 20
#define SIZE_IMAGE_HEIGHT 30

IplImage*grayScale(IplImage*src) {
	IplImage * grayScaleImg = cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	cvCvtColor(src, grayScaleImg, CV_RGB2GRAY);
	return grayScaleImg;
}

IplImage* ImageRestoration(IplImage * src)
{
	int w = src->width;
	int h = src->height;

	IplImage *mImg = cvCreateImage(cvSize(w / 2, h / 2), IPL_DEPTH_8U, 1);		// Anh su dung cho bien doi hinh thai hoc
	IplImage *src_pyrdown = cvCreateImage(cvSize(w / 2, h / 2), IPL_DEPTH_8U, 1);
	IplImage *tmp = cvCreateImage(cvSize(w / 2, h / 2), IPL_DEPTH_8U, 1);
	IplImage *thresholed = cvCreateImage(cvSize(w / 2, h / 2), IPL_DEPTH_8U, 1);	// Anh nhi phan voi nguong
	IplImage *mini_thresh = cvCreateImage(cvSize(w / 2, h / 2), IPL_DEPTH_8U, 1);
	IplImage *dst = cvCreateImage(cvSize(w / 2, h / 2), IPL_DEPTH_8U, 1);			// Anh lam ro vung bien so

	cvPyrDown(src, src_pyrdown);
	//cvShowImage("pyr", src_pyrdown);
	
	cvMorphologyEx(src_pyrdown, mImg, tmp, S2, CV_MOP_BLACKHAT);
	//cvShowImage("mImg", mImg);
	cvNormalize(mImg, mImg, 0, 255, CV_MINMAX);
	//cvShowImage("mimg2", mImg);
	cvWaitKey(20);
	// Nhi phan hoa anh mImg
	cvThreshold(mImg, thresholed, (int)10 * cvAvg(mImg).val[0], 255, CV_THRESH_BINARY);
	cvZero(dst);
	cvCopy(thresholed, mini_thresh);
	cvShowImage("threshold", thresholed);
	cvWaitKey(20);
	// Su dung hinh chu nhat co size = 8x16 truot tren toan bo anh

	int cnt;
	int nonZero1, nonZero2, nonZero3, nonZero4;
	CvRect rect;

	for (int i = 0; i < mini_thresh->width - 32; i += 4)
	{
		for (int j = 0; j < mini_thresh->height - 16; j += 4)
		{
			rect = cvRect(i, j, 16, 8);
			cvSetImageROI(mini_thresh, rect);	//ROI = Region of Interest
			nonZero1 = cvCountNonZero(mini_thresh);
			cvResetImageROI(mini_thresh);

			rect = cvRect(i + 16, j, 16, 8);
			cvSetImageROI(mini_thresh, rect);	//ROI = Region of Interest
			nonZero2 = cvCountNonZero(mini_thresh);
			cvResetImageROI(mini_thresh);

			rect = cvRect(i, j + 8, 16, 8);
			cvSetImageROI(mini_thresh, rect);	//ROI = Region of Interest
			nonZero3 = cvCountNonZero(mini_thresh);
			cvResetImageROI(mini_thresh);

			rect = cvRect(i + 16, j + 8, 16, 8);
			cvSetImageROI(mini_thresh, rect);	//ROI = Region of Interest
			nonZero4 = cvCountNonZero(mini_thresh);
			cvResetImageROI(mini_thresh);

			cnt = 0;
			if (nonZero1 > 15) { cnt++; }
			if (nonZero2 > 15) { cnt++; }
			if (nonZero3 > 15) { cnt++; }
			if (nonZero4 > 15) { cnt++; }

			if (cnt > 2)
			{
				rect = cvRect(i, j, 32, 16);
				cvSetImageROI(dst, rect);
				cvSetImageROI(mini_thresh, rect);
				cvCopy(mini_thresh, dst);
				cvResetImageROI(dst);
				cvResetImageROI(mini_thresh);
			}
		}
	}

	IplImage* dst_clone = cvCloneImage(dst);

	cvDilate(dst, dst, NULL, 2);
	cvErode(dst, dst, NULL, 2);
	cvDilate(dst, dst, S1, 9);
	cvErode(dst, dst, S1, 10);
	cvDilate(dst, dst);
	return dst;
	//cvShowImage("Source" , src);
	//cvShowImage("mImg", mImg);
	//cvShowImage("mini_thresh", mini_thresh);
	//cvShowImage("dst_clone", dst_clone);
	//cvShowImage("dst", dst);
	//cvWaitKey();
}

IplImage * resizex2(IplImage * src)
{
	IplImage * imgResize = cvCreateImage(CvSize(2 * src->width, 2 * src->height), src->depth, src->nChannels);
	cvResize(src, imgResize, CV_INTER_LINEAR);
	return imgResize;
}

IplImage * resize(IplImage * src, int x, int y)
{
	IplImage * imgReszie = cvCreateImage(CvSize(x, y), src->depth, src->nChannels);
	cvResize(src,imgReszie,NULL);
	return imgReszie;
}

int CountContour(IplImage * plate)
{
	int cnt = 0;
	IplImage *resizeImg, *binaryImg, *grayimg,*cloneBinaryImg;
	resizeImg = cvCreateImage(cvSize(408, 70), IPL_DEPTH_8U, 3);
	binaryImg = cvCreateImage(cvSize(408, 70), IPL_DEPTH_8U, 1);
	cloneBinaryImg = cvCreateImage(cvSize(408, 70), IPL_DEPTH_8U, 1);
	grayimg = cvCloneImage(plate);
	grayimg = resize(grayimg, 408, 70);

	cvResize(plate, resizeImg);
	cvCvtColor(resizeImg, binaryImg, CV_RGB2GRAY);
	cvAdaptiveThreshold(binaryImg, binaryImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 13, 2);
	// clone binaryImg to cloneBinaryimg
	cloneBinaryImg = cvCloneImage(binaryImg);
	//
	//cvShowImage("binaryImg", binaryImg);
	// khai bao luu tru so xe
	CvMemStorage *storage = cvCreateMemStorage(0);
	CvSeq *contours = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), storage);
	cvFindContours(binaryImg, storage, &contours);
	//
	//cvShowImage("contour", binaryImg);
	while (contours)
	{
		//int xmax, xmin, ymax, ymin;
		CvRect rect = cvBoundingRect(contours);
		//cvRectangle(grayimg, CvPoint(rect.x, rect.y), CvPoint(rect.x + rect.width, rect.y + rect.height), GREEN);
		if (rect.height>40&&rect.height<65) {
			if (rect.width>15&&rect.width<50)
			{
				cvRectangle(grayimg, CvPoint(rect.x, rect.y), CvPoint(rect.x + rect.width, rect.y + rect.height), RED);
				cnt++;
			}
			
		}
		contours = contours->h_next;
	}
	

	cvShowImage("cloneBinaryImg",cloneBinaryImg);
	cvShowImage("cont", grayimg);
	cvShowImage("binary",binaryImg);
	cvWaitKey();
	return cnt;
}//revise
// this funtion is counting the bouding

vector<IplImage*> FindCharacter(IplImage * plate)
{
	vector<IplImage*> charImgVector;
	vector<CvRect> rect;
	//
	IplImage *resizeImg, *binaryImg, *cloneImg;

	resizeImg = cvCreateImage(cvSize(408, 70), IPL_DEPTH_8U, 3);
	binaryImg = cvCreateImage(cvSize(408, 70), IPL_DEPTH_8U, 1);

	cvResize(plate, resizeImg);
	cvCvtColor(resizeImg, binaryImg, CV_RGB2GRAY);
	cvAdaptiveThreshold(binaryImg, binaryImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 13, 2);

	cloneImg = cvCloneImage(resizeImg);

	CvMemStorage *storage = cvCreateMemStorage(0);
	CvSeq *contours = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), storage);
	cvFindContours(binaryImg, storage, &contours);
	//cvShowImage("1",binaryImg);
	//double ratio, ratioWhite;
	//int s, white;
	while (contours)
	{
		//int xmax, xmin, ymax, ymin;
		CvRect r = cvBoundingRect(contours);
		//cvRectangle(grayimg, CvPoint(rect.x, rect.y), CvPoint(rect.x + rect.width, rect.y + rect.height), GREEN);
		if (r.height > 40 && r.height < 65) {
			if (r.width > 15 && r.width < 50)
			{
				cvRectangle(cloneImg, CvPoint(r.x, r.y), CvPoint(r.x + r.width, r.y + r.height), RED);
				rect.push_back(r);
			}

		}
		contours = contours->h_next;
	}
	// sap sep lai thu tu cua image
	for (int i = 0; i < rect.size() - 1; i++)
	{
		for (int j = i + 1; j < rect.size(); j++) {

			CvRect curr;
			curr = rect[i];
			if (rect[i].x > rect[j].x)// doi cho
			{
				rect[i] = rect[j];
				rect[j] = curr;
			}
		}
	}
	//
	// cut image
	IplImage *charImg;
	IplImage *saveImg;
	for (int i = 0; i < rect.size(); i++)
	{
		charImg = cvCreateImage(cvSize(rect[i].width, rect[i].height), IPL_DEPTH_8U, 3);
		cvSetImageROI(resizeImg, rect[i]);
		cvCopy(resizeImg, charImg, NULL);
		cvResetImageROI(resizeImg);
		//resizeImg = resize(resizeImg, SIZE_IMAGE_WIDTH, SIZE_IMAGE_HEIGHT);
		//add image to vector
		charImgVector.push_back(charImg);
		//
		/*char name[8];
		sprintf(name, "Anh %d", i + 1);
		cvShowImage(name, charImg);
		cvWaitKey(20);*/
		//cvShowImage(name, charImg);


		// Luu anh lam mau de training
		saveImg = cvCreateImage(cvSize(rect[i].width, rect[i].height), IPL_DEPTH_8U, 1);
		cvCvtColor(charImgVector[i], saveImg, CV_RGB2GRAY);
		cvAdaptiveThreshold(saveImg, saveImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 13, 2);

		//char sname[8];
		//sprintf(sname, "Data/%d.jpg", i + 1);
		////cvShowImage(sname, saveImg);
		//cvSaveImage(sname, saveImg);
	}
	
	//cvShowImage("cloneImg", charImgVector.at(2));
	//cvWaitKey();

	return charImgVector;
}

IplImage* FindPlate(IplImage * src)
{
	IplImage* plate;
	IplImage* contourImg = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);	// anh tim contour
	IplImage* grayImg = grayScale(src);//anh xam

	IplImage* cloneImg = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
	cloneImg = cvCloneImage(src);
	plate = cvCloneImage(src);
	// tien xu ly anh
	cvCopy(grayImg, contourImg);
	cvNormalize(contourImg, contourImg, 0, 255, CV_MINMAX);
	contourImg = ImageRestoration(contourImg);
	contourImg = resizex2(contourImg);
	IplImage* rectImg = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
	//cvMerge(contourImg, contourImg, contourImg, NULL, rectImg); // tron anh
	//cvShowImage("window", src);
	cvShowImage("contourImg1", contourImg);
	cvWaitKey(20);

	CvMemStorage *storagePlate = cvCreateMemStorage(0);
	CvSeq *contours = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), storagePlate);
	cvFindContours(contourImg, storagePlate, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
	// resize contour
	
	int xmin, ymin, xmax, ymax, w, h, s, r;
	int count;
	double ratio;	// ty le chieu rong tren chieu cao
	CvRect rectPlate;
	// khoi tao mang de luu cac trang thai bien so
	IplImage** plateArr = new IplImage *[5];
	int j = 0;
	for (int i = 0; i < 5; i++)
	{
		plateArr[i] = NULL;
	}
	while (contours) {
		count = contours->total;
		CvPoint *PointArray = new CvPoint[count];
		cvCvtSeqToArray(contours, PointArray, CV_WHOLE_SEQ);

		for (int i = 0; i < count; i++)
		{
			if (i == 0)
			{
				xmin = xmax = PointArray[i].x;
				ymin = ymax = PointArray[i].y;
			}

			if (PointArray[i].x > xmax) {
				xmax = PointArray[i].x;
			}
			if (PointArray[i].x < xmin) {
				xmin = PointArray[i].x;
			}

			if (PointArray[i].y > ymax) {
				ymax = PointArray[i].y;
			}
			if (PointArray[i].y < ymin) {
				ymin = PointArray[i].y;
			}
		}
		w = xmax - xmin;
		h = ymax - ymin;
		s = w * h;
		cvRectangle(cloneImg, cvPoint(xmin, ymin), cvPoint(xmax,ymax), UNKNOW); 

		if (s != 0)
		{
			r = (contourImg->width*contourImg->height) / s;
		} 
		else
		{
			r = 1000;
		}

		if (w == 0 || h == 0)
		{
			ratio = 0;
		} 
		else
		{
			ratio = (double)(w / h);
		}

		if (r > 30 && r < 270) 
		{
			cvRectangle(cloneImg,cvPoint(xmin,ymin),cvPoint(xmax,ymax),BLUE);
			if (ratio > 2.6&&ratio < 7)
			{
				cvRectangle(cloneImg, cvPoint(xmin, ymin), cvPoint(xmax, ymax), GREEN);
				if (w > 80 && w < 250 && h > 25 && h < 150)
				{
					rectPlate = cvRect(xmin, ymin, w, h);
					cvRectangle(cloneImg, cvPoint(rectPlate.x, rectPlate.y), cvPoint(rectPlate.x + rectPlate.width, rectPlate.y + rectPlate.height), RED, 2);
				
					cvSetImageROI(plate, rectPlate);
					/*int cnt = CountCharacter(plate);
					if (cnt >= 5) {
						plateArr[j] = cvCloneImage(plate);
						j++;
					}*/
				}
			} 
		}

		contours = contours->h_next;
	}
	////cvShowImage("window", cloneImg);
	//cvShowImage("plate123", plate);
	//cvWaitKey(20);
	return plate;
}
//void recognictionCharacter(vector<IplImage*> src)
//{
//	// read database 
//	CvMat *classificationMat = (CvMat*)cvLoad("classifications.xml");
//	CvMat *imagesMat = (CvMat*)cvLoad("images.xml");
//
//	//todo
//}

CvMat * convertIplimagetoCvMat(IplImage * src)
{
	CvMat *convertMat = cvCreateMat(SIZE_IMAGE_HEIGHT, SIZE_IMAGE_WIDTH, CV_32FC1);//check
	float val;
	for (size_t i = 0; i < src->height; i++)
	{
		uchar* ptr = (uchar*)(src->imageData + i * src->widthStep);
		for (size_t j = 0; j < src->width; j++)
		{
			val = (float)(ptr[3 * j]);
			val = float(255) - val;
			*((float*)(CV_MAT_ELEM_PTR(*convertMat, i, j))) = val;
		}
	}
	return convertMat;
}

void showCvMatFloat(CvMat*src)
{
	float element;
	for (size_t i = 0; i < src->height; i++)
	{
		for (size_t j = 0; j < src->width; j++)
		{
			element = CV_MAT_ELEM(*src, float, i, j);
			cout << element << " ";
		}
		cout << endl;
	}
}//
void showCvMatFloatAbs(CvMat*src)
{
	float element;
	for (size_t i = 0; i < src->height; i++)
	{
		for (size_t j = 0; j < src->width; j++)
		{
			element = CV_MAT_ELEM(*src, float, i, j);
			if (element < 0) {
				element = abs(element);
			}
			cout << element << " ";
		}
		cout << endl;
	}
}
int getDataClassifications(int x)
{
	vector<int> vectorIntData[5];
	CvMat *fs = (CvMat*)cvLoad("classifications.xml");
	int data;
	for (size_t i = 0; i < fs->height; i++)
	{
		data = CV_MAT_ELEM(*fs,int, i, 0);
		if (x == i) {
			return data;
		}
	}
	return -1;
}
//
float sumMat(CvMat*src) {
	float sum = 0;
	float currVal;
	for (size_t i = 0; i < src->height; i++)
	{
		for (size_t j = 0; j < src->width; j++)
		{
			currVal = CV_MAT_ELEM(*src, float, i, j);
			if (currVal < 0) {
				currVal = abs(currVal);
			}
			sum += currVal;
		}
	}
	return sum;
}

vector<IplImage*> resizeVectorImage(vector<IplImage*> src)
{
	IplImage* curr;
	vector<IplImage*> newVectorImg;
	IplImage* grayImg;

	for (size_t i = 0; i < src.size(); i++)
	{
		curr = src.at(i);
		curr = resize(curr, SIZE_IMAGE_WIDTH, SIZE_IMAGE_HEIGHT);
		grayImg = grayScale(curr);
		cvAdaptiveThreshold(grayImg, grayImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 13, 2);
		newVectorImg.push_back(grayImg);
	}
	return newVectorImg;
}

void showImgvector(vector<IplImage*> src)
{
	IplImage* curr;
	vector<IplImage*> vectorReturn;
	//int s = src.size();
	
	
	for (size_t i = 0; i < src.size(); i++)
	{
		string data;
		curr = src.at(i);
		data = "Data/" + to_string(i) + ".jpg";
		char* characterData = (char*)data.c_str();
		//cvShowImage(sname, curr);
		cvSaveImage(characterData, curr);
		vectorReturn.push_back(curr);
		
	}
	//cvWaitKey();
}

void showIplimagedata(IplImage*src) {

	float val;
	for (size_t i = 0; i < src->height; i++)
	{
		uchar* ptr = (uchar*)(src->imageData + i * src->widthStep);

		for (size_t i = 0; i < src->width; i++)
		{
			val = (float)(ptr[3 * i]);
			cout << val << "   ";
		}
		cout << endl;
	}
}
void showcharacterFinal(vector<IplImage*> src)
{
	string stringReturn;
	CvMat *fs = (CvMat*)cvLoad("classifications.xml");
	CvMat *imgData = (CvMat*)cvLoad("images.xml");
	//for (size_t count = 0; count < src.size(); count++)
	//{

		IplImage *currentImage;
		CvMat * matResultSubtract = cvCreateMat(30, 20, CV_32FC1);
		CvMat * matResultSubtractAbs = cvCreateMat(30, 20, CV_32FC1);
		//IplImage *grayImg;

		// add data to imgDataPart
		//for (int k = 0; k < 180; k++) {

		CvMat *imgDataPart = cvCreateMat(30, 20, CV_32FC1);
		CvMat *vectorImgDataPart = cvCreateMat(30, 20, CV_32FC1);
		float currentData;
		// load image form Data to iplimage//
		for (int valint = 0; valint < 8; valint++)
		{

			currentImage = loadImgData(valint);
			vectorImgDataPart = convertIplimagetoCvMat(currentImage);
			//vectorImgDataPart = convertMat(vectorImgDataPart);
			//showCvMatFloat(vectorImgDataPart);
			//cout << "//////////////////////////////////////////////" << endl;
			// end load image ////////////////////////////////////////////////////
			// take data form images.xml//
			// for loop 180

			float compareVal;
			int idx;
			for (int count180 = 0; count180 < 181; count180++)
			{
				float sumMatrix = 0;
				for (int i = 0; i < 30; i++)
				{
					for (int j = 0; j < 20; j++)
					{
						currentData = CV_MAT_ELEM(*imgData, float, count180, (i * 20 + j));
						*((float*)(CV_MAT_ELEM_PTR(*imgDataPart, i, j))) = currentData;
					}
				}//////////////////*******************/////////////////// return imgDataPart
				cvSub(vectorImgDataPart, imgDataPart, matResultSubtract, 0);
				sumMatrix = sumMat(matResultSubtract);
				if (count180 == 0)
				{
					compareVal = sumMatrix;
					idx = 0;
				}
				else
				{
					if (sumMatrix <= compareVal) {
						compareVal = sumMatrix;
						idx = count180;
					}
				}
			}
			//////////////////*******************/////////////////// return imgDataPart
			//test code
			//cvAbs(matResultSubtract,matResultSubtractAbs);
			/////
			//cout << "String   " << char(getDataClassifications(idx)) << endl;
			stringReturn += char(getDataClassifications(idx));
			
			//}
			//return string();
		}
		cout <<" BIEN SO CUA XE LA :"<< stringReturn << endl;
}
// GOOD
IplImage* loadImgData(int x)
{
	char *sname;
	IplImage*imgCurr;
	string result = "Data/"+to_string(x)+".jpg";
	sname = (char*)result.c_str();
	imgCurr = cvLoadImage(sname);
	//cvShowImage("1",imgCurr);
	//cvWaitKey(20);
	//cout << sname << endl;
	return imgCurr;
}
//CvMat* convertMat(CvMat*src) {
//	float currVal;
//	float curr;
//	CvMat *matReturn = cvCreateMat(src->height, src->width, CV_32FC1);
//	for (int i=0;i<src->height;i++)
//	{
//		for (int j =0;j<src->width;j++)
//		{
//			currVal = CV_MAT_ELEM(*src, float, i, j);
//			curr = 255. - currVal;
//			*((float*)(CV_MAT_ELEM_PTR(*matReturn, i, j)));
//		}
//	}
//	return matReturn;
//};

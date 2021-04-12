#pragma once

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "segmentation.h"

using namespace std;
using namespace cv;


Mat PickROIForNumberRecognition(Mat image);

bool Card_is2(double* hu);
bool Card_is3(double* hu);
bool Card_is5(double* hu);
bool Card_is6(double* hu);
bool Card_is7(double* hu);
bool Card_is8(double* hu);
bool Card_is9(double* hu);
bool Card_is10(double* hu);

vector<Moments> CalculateImageMoments(vector<vector<Point> > contours);
void CalculateHuMoments(Moments contour_moment, double* &hu);
int* CreateNumbersTable();
void CheckNumber(int* numbers_table, double* hu);
void SetCardNumber(int* numbers_table, Card &recognised_card);
void NumberRecognition(Mat image, Card &recognised_card);

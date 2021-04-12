#pragma once

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#include "segmentation.h"

using namespace std;
using namespace cv;

Mat PickROIForSymbolRecognition(Mat image);

bool Card_isClubs(double* hu);
bool Card_isDiamonds(double* hu);
bool Card_isHearts(double* hu);
bool Card_isSpades(double* hu);

int* CreateSymbolsTable();
void CheckSymbol(int* symbols_table, double* hu);
void SetCardSymbol(int* symbols_table, Card &recognised_card);
void SymbolRecognition(Mat image, Card &recognised_card);
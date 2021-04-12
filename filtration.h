#pragma once

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#include "segmentation.h"

using namespace std;
using namespace cv;

Mat OpenImage();

//True if empty
bool EmptyImage(Mat image);

//Enlarge in y direction (more rows)
Mat EnlargeImage(Mat image);

//To remove salt & pepper noise
Mat MedianFilter(Mat image);

//Unsharp mask
Mat SharpenImage(Mat image);

//Look-up table gamma correction to improve contrast
Mat GammaCorrection(Mat image_filtered);

Mat ApplyFilters(Mat image);

Mat Binarization(Mat image);
Mat CannyEdgeDetection(Mat image_binary);
void ClosingOperation(Mat &image);
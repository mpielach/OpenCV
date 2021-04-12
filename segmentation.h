#pragma once

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

struct Card
{
	Mat perspective_image;
	int number;
	string symbol;
};

void RecogniseCards();

vector<vector<Point>> FindingContours(Mat image_binary);

Rect FindLargestContour(vector<vector<Point> > contours, vector<vector<Point> > &temporary);
Rect* FindCardsContours(vector<vector<Point> > contours);

//Sort by distance from (0,0)
bool SortPoints(Point2f pt_a, Point2f pt_b);
void SortVector(vector<Point2f> &perspective_points);

//Enlarge card image in xy direction and move card to the middle of an image
Mat TranslateCardImage(Mat image);

vector<Point2f> FindCardCorners(vector<vector<Point> > contours);

Mat CreatePerspectiveImage(Mat image_resized, vector<Point2f> image_corners, vector<Point2f> card_corners);
Mat PerspectiveTransformation(Mat image);

#pragma once

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "segmentation.h"

using namespace std;
using namespace cv;


string FileName();

void MessageWrongFile();

void DisplayImage(Mat image);

void MessageRecognisedCards(Card* detected_cards);
void DisplayRecognisedCard(Card recognised_card);
void DisplaySortedCards(Card* detected_cards);
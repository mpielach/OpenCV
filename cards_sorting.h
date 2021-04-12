#pragma once

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#include "segmentation.h"

using namespace std;
using namespace cv;

//Increasing sorting
bool SortCardsByNumber(Card card_a, Card card_b);
void SortCards(Card* &detected_cards);
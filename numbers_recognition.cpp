#include "interface.h"
#include "numbers_recognition.h"
#include "symbols_recognition.h"
#include "filtration.h"

Mat PickROIForNumberRecognition(Mat image)
{
	Mat ROI_Card; //Upper-left part of the card which includes card's number
	Rect ROI(0, 0, 0.25*image.cols, 0.33*image.rows); //Arbitrary values according to number's location on cards
	ROI_Card = image(ROI).clone();
	return ROI_Card;
}

bool Card_is2(double* hu)
{
	if (												//Looking for '2'
		(hu[0] > 0.44 and hu[0] < 0.52 and
			hu[1] > 0.09 and hu[1] < 0.18 and
			hu[2] > 0.0025 and hu[2] < 0.005 and
			hu[3] > 0.0005 and hu[3] < 0.002 and
			hu[5] < 0 and
			hu[6] > 0)
		or												//Looking for central part of 2 (similar to '?')
		(hu[0] > 0.28 and hu[0] < 0.35 and
			hu[1] > 0.013 and hu[1] < 0.03 and
			hu[2] > 0.0006 and hu[2] < 0.005 and
			hu[3] > 0.0001 and hu[3] < 0.0069 and
			hu[4] < 0 and
			hu[5] < 0)
		or												//Looking for bottom part of 2 (horizontal line)
		(hu[0] > 0.28 and hu[0] < 0.35 and
			hu[1] > 0.0377 and hu[1] < 0.038 and
			hu[2] > 0.0006 and hu[2] < 0.005 and
			hu[3] > 0.0001 and hu[3] < 0.0069 and
			hu[4] < 0 and
			hu[5] < 0))
	{
		return true;
	}
	else return false;
}

bool Card_is3(double* hu)
{
	if (											//Looking for '3'
		hu[0] > 0.315 and hu[0] < 0.365 and
		hu[1] > 0.02 and hu[1] < 0.06 and
		hu[2] > 0.0022 and hu[2] < 0.0073 and
		hu[3] > 0.0007 and hu[3] < 0.002)
	{
		return true;
	}
	else return false;
}

bool Card_is5(double* hu)
{
	if (												//Looking for '5'
		(hu[0] > 0.41 and hu[0] < 0.47 and
			hu[1] > 0.03 and hu[1] < 0.085 and
			hu[2] > 0.001 and hu[2] < 0.023 and
			hu[3] > 0.00005 and hu[3] < 0.00075)
		or												//Looking for bottom part of '5'
		(hu[0] > 0.19 and hu[0] < 0.196 and
			hu[1] > 0.009 and hu[1] < 0.011 and
			hu[5] < 0 and
			hu[6] < 0)
		or												//Looking for upper part of '5'
		(hu[0] > 0.29 and hu[0] < 0.34 and
			hu[1] > 0.013 and hu[1] < 0.02 and
			hu[2] > 0.0005 and hu[2] < 0.0008 and
			hu[3] > 1e-5 and hu[3] < 1e-4 and
			hu[5] < 0))
	{
		return true;
	}
	else return false;
}

bool Card_is6(double* hu)
{
	if (												//Looking for '6'
		(hu[0] > 0.17 and hu[0] < 0.205 and
			hu[1] > 0.00085 and hu[1] < 0.0029 and
			hu[2] > 7e-4 and hu[2] < 2e-3 and
			hu[3] > 0.0001 and hu[3] < 0.00053 and
			hu[6] < 0)
		or												//Looking for upper part of 6 (disconnected from bottom part)
		(hu[0] > 0.3 and hu[0] < 0.317 and
			hu[1] > 0.005 and hu[1] < 0.006 and
			hu[2] > 0.0045 and hu[2] < 0.006 and
			hu[6] > 0))
	{
		return true;
	}
	else return false;
}

bool Card_is7(double* hu)
{
	if (											//Looking for '7'
		hu[0] > 0.315 and hu[0] < 0.395 and
		hu[1] > 0.02 and hu[1] < 0.05 and
		hu[2] > 0.01 and hu[2] < 0.03 and
		hu[3] > 0.0013 and hu[3] < 0.0047 and
		hu[6] < 0)
	{
		return true;
	}
	else return false;
}

bool Card_is8(double* hu)
{
	if (											//Looking for '8'
		(hu[0] > 0.165 and hu[0] < 0.19 and
			hu[1] > 0.0015 and hu[1] < 0.0049 and
			hu[2] > 2e-5 and hu[2] < 2e-4 and
			hu[3] > 6e-7 and hu[3] < 5e-6 and
			hu[6] < 1e-10)
		or											//Looking for '0' (lines inside 8 are not connected)
		(hu[0] > 0.32 and hu[0] < 0.33 and
			hu[1] > 0.07 and hu[1] < 0.08 and
			hu[4] > 0 and
			hu[5] > 1e-8 and
			hu[6] > 0))
	{
		return true;
	}
	else return false;
}

bool Card_is9(double* hu)
{
	if (												//Looking for '9'
		hu[0] > 0.188 and hu[0] < 0.195 and
		hu[1] > 0.002 and hu[1] < 0.0032 and
		hu[2] > 0.00084 and hu[2] < 0.0009 and
		hu[3] > 0.0001 and hu[3] < 0.00038 and
		hu[4] > 0 and
		hu[5] > 0 and
		hu[6] < 0)
	{
		return true;
	}
	else return false;
}

bool Card_is10(double* hu)
{
	if (											// Looking for '1'
		(hu[0] > 0.23 and hu[0] < 0.25 and
			hu[1] > 0.008 and hu[1] < 0.011 and
			hu[2] > 2e-6 and hu[2] < 2.9e-5 and
			hu[3] > 0.0005 and hu[3] < 0.00099)
		or											// Looking for 1 and 0 connected into one contour '10'
		(hu[0] > 0.33 and hu[0] < 0.359 and
			hu[1] > 0.033 and hu[1] < 0.055 and
			hu[2] > 0.002 and hu[2] < 0.005 and
			hu[3] > 0.0002 and hu[3] < 0.0004)
		)
	{
		return true;
	}
	else return false;
}


void CheckNumber(int* numbers_table, double* hu)
{
	if (Card_is2(hu))
	{
		numbers_table[2]++;
	}
	if (Card_is3(hu))
	{
		numbers_table[3]++;
	}
	if (Card_is5(hu))
	{
		numbers_table[5]++;
	}
	if (Card_is6(hu))
	{
		numbers_table[6]++;
	}
	if (Card_is7(hu))
	{
		numbers_table[7]++;
	}
	if (Card_is8(hu))
	{
		numbers_table[8]++;
	}
	if (Card_is9(hu))
	{
		numbers_table[9]++;
	}
	if (Card_is10(hu))
	{
		numbers_table[10]++;
	}
}

void SetCardNumber(int* numbers_table, Card &recognised_card)
{
	if (numbers_table[2] > 0)
	{
		recognised_card.number = 2;
	}
	if (numbers_table[3] > 0)
	{
		recognised_card.number = 3;
	}
	if (numbers_table[5] > 0)
	{
		recognised_card.number = 5;
	}
	if (numbers_table[6] > 0 and numbers_table[9] == 0) //Make sure that '6' is not mistaken with '9'
	{
		recognised_card.number = 6;
	}
	if (numbers_table[7] > 0)
	{
		recognised_card.number = 7;
	}
	if (numbers_table[8] > 0)
	{
		recognised_card.number = 8;
	}
	if (numbers_table[9] > 0)
	{
		recognised_card.number = 9;
	}
	if (numbers_table[10] > 0)
	{
		recognised_card.number = 10;
	}
}

vector<Moments> CalculateImageMoments(vector<vector<Point> > contours)
{
	vector<Moments> image_moments(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		image_moments[i] = moments(contours[i], true); //True because image is binary!
	}
	return image_moments;
}

void CalculateHuMoments(Moments contour_moment, double* &hu)
{
	HuMoments(contour_moment, hu);
}

int* CreateNumbersTable()
{
	int* numbers_table = new int[11]; // Index in tab represents card's number
	for (int i = 0; i < 11; i++)
	{
		numbers_table[i] = 0;
	}

	return numbers_table;
}

void NumberRecognition(Mat image, Card &recognised_card)
{
	Mat image_binary;
	image_binary = Binarization(image);

	vector<vector<Point> > contours = FindingContours(image_binary);
	vector<Moments> image_moments = CalculateImageMoments(contours);

	int* numbers_table = CreateNumbersTable();

	//Calculate Hu moments and check card number
	for (int i = 0; i < contours.size(); i++)
	{
		double *hu = new double[7];
		CalculateHuMoments(image_moments[i], hu);
		CheckNumber(numbers_table, hu);
		delete[] hu;
	}
	SetCardNumber(numbers_table, recognised_card);

	delete[] numbers_table;
}

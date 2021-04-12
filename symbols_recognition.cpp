#include "symbols_recognition.h"
#include "interface.h"
#include "numbers_recognition.h"
#include "filtration.h"

Mat PickROIForSymbolRecognition(Mat image)
{
	Mat ROI_Card; //Center of card which includes symbols
	Rect ROI(0.199*image.cols, 0.199 * image.rows , 0.6*image.cols, 0.62*image.rows); //Arbitrary values
	ROI_Card = image(ROI).clone();
	return ROI_Card;
}

bool Card_isClubs(double* hu)
{
	if (
		hu[0] > 0.2 and hu[0] < 0.25 and
		hu[1] > 0.005 and hu[1] < 0.015 and 
		hu[2] > 0.002 and hu[2] < 0.005 and
		hu[3] > 0.00007 and hu[3] < 0.00026 and
		hu[4] < 0 and
		hu[5] < 0)
	{
		return true;
	}
	else return false;
}

bool Card_isDiamonds(double* hu)
{
	if (
		hu[0] > 0.16 and hu[0] < 0.179 and
		hu[1] < 3e-4 and
		hu[2] < 8e-6 and
		hu[3] < 1e-7 and
		hu[4] < 1e-10 and
		hu[6] < 1e-11)
	{
		return true;
	}
	else return false;
}

bool Card_isHearts(double* hu)
{
	if (
		hu[0] > 0.18 and hu[0] < 0.196 and
		hu[1] > 0.0007 and hu[1] < 0.0051 and
		hu[2] > 0.001 and hu[2] < 0.003)
	{
		return true;
	}
	else return false;
}

bool Card_isSpades(double* hu)
{
	if (hu[0] > 0.19 and hu[0] < 0.22 and
		hu[1] > 0.0051 and hu[1] < 0.02 and
		hu[2] > 1e-5 and hu[2] < 0.00246 and
		hu[3] < 1e-4 and
		hu[4] < 0 and
		hu[5] < 0)
	{
		return true;
	}
	else return false;
}

void CheckSymbol(int* symbols_table, double* hu)
{
	if (Card_isClubs(hu))
	{
		symbols_table[0]++;
	}
	if (Card_isDiamonds(hu))
	{
		symbols_table[1]++;
	}
	if (Card_isSpades(hu))
	{
		symbols_table[3]++;
	}
	if (Card_isHearts(hu))
	{
		symbols_table[2]++;
	}
}

void SetCardSymbol(int* symbols_table, Card &recognised_card)
{
	if (symbols_table[0] > 0)
	{
		recognised_card.symbol = "Clubs";
	}
	if (symbols_table[1] > 0)
	{
		recognised_card.symbol = "Diamonds";
	}
	if (symbols_table[3] > 0)
	{
		recognised_card.symbol = "Spades";
	}
	if (symbols_table[2] > 0)
	{
		recognised_card.symbol = "Hearts";
	}

}

int* CreateSymbolsTable()
{
	int* symbols_table = new int[4]; //4 symbols possible
	for (int i = 0; i < 4; i++)
	{
		symbols_table[i] = 0;
	}
	return symbols_table;
}

void SymbolRecognition(Mat image, Card &recognised_card)
{
	Mat image_binary;
	image_binary = Binarization(image);

	vector<vector<Point> > contours = FindingContours(image_binary);
	vector<Moments> image_moments = CalculateImageMoments(contours);

	int* symbols_table = CreateSymbolsTable();

	//Calculate Hu moments and check symbol
	for (int i = 0; i < contours.size(); i++)
	{
		double *hu = new double[7];
		CalculateHuMoments(image_moments[i], hu);

		if (contours[i].size() > 20) //Avoid checking too small contours (noise)
		{
			CheckSymbol(symbols_table, hu);
		}
		delete[] hu;
	}
	SetCardSymbol(symbols_table, recognised_card);

	delete[] symbols_table;
}
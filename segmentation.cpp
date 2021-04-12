#include "segmentation.h"
#include "interface.h"
#include "numbers_recognition.h"
#include "symbols_recognition.h"
#include "cards_sorting.h"
#include "filtration.h"

vector<vector<Point>> FindingContours(Mat image_binary)
{
	Mat image_edges = CannyEdgeDetection(image_binary);
	ClosingOperation(image_edges);

	vector<vector<Point> > contours; 
	vector<Vec4i> hierarchy; 
	findContours(image_edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	return contours;
}

Rect FindLargestContour(vector<vector<Point> > contours, vector<vector<Point> > &temporary)
{
	int largest_area = 0;
	int largest_contour_index = 0;
	Rect max_element; // Bounding rectangle for the biggest contour

	for (int i = 0; i < temporary.size(); i++)
	{
		double area = contourArea(temporary[i], false); 
		if (area > largest_area)
		{
			largest_area = area;
			largest_contour_index = i;                
			max_element = boundingRect(contours[i]);
		}
	}

	temporary[largest_contour_index] = { Point(0, 0) }; // Delete maximum contour which was found
	return max_element;
}

Rect* FindCardsContours(vector<vector<Point> > contours)
{
	vector<vector<Point> > temporary = contours; //Copy of contours

	int largest_area = 0;
	int largest_contour_index = 0;
	Rect* tab_rect = new Rect[8];

	//Have to find 8 max contours because every card has 2 contours: inside and outside
	for (int i = 0; i < 8; i++)
	{
		tab_rect[i] = FindLargestContour(contours, temporary);
	}

	return tab_rect;
}

bool SortPoints(Point2f pt_a, Point2f pt_b)
{
	double r_a = sqrt(pt_a.x*pt_a.x + pt_a.y*pt_a.y);
	double r_b = sqrt(pt_b.x*pt_b.x + pt_b.y*pt_b.y);
	return (r_a < r_b);
}

void SortVector(vector<Point2f> &perspective_points)
{
	sort(perspective_points.begin(), perspective_points.end(), SortPoints);
}

Mat TranslateCardImage(Mat image)
{
	Mat image_resized;
	image_resized = Mat::zeros(image.rows + 10, image.cols + 10, image.type()); //Enlarge in both directions
	Rect im_original(0, 0, image.cols, image.rows);
	image.copyTo(image_resized(im_original));

	//Translate by [5,5] to the middle of an image, so that corners and edges of card don't touch corners of image
	Mat translation_matrix = (Mat_<double>(2, 3) << 1, 0, 5, 0, 1, 5);
	warpAffine(image_resized, image_resized, translation_matrix, image_resized.size());

	return image_resized;
}

vector<Point2f> FindCardCorners(vector<vector<Point> > contours)
{
	vector<Point2f> card_corners(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		if (contourArea(contours[i]) > 10000) //Arbitrary size, looking for the largest contour
		{
			approxPolyDP(Mat(contours[i]), card_corners, 80, true); //80 is arbitrary - goal is to find 4 corners
		}
	}

	return card_corners;
}

Mat CreatePerspectiveImage(Mat image_resized, vector<Point2f> image_corners, vector<Point2f> card_corners)
{
	Mat image_perspective;
	image_resized.convertTo(image_perspective, CV_32F, 1.0 / 255.0);

	Mat perspective_kernel = getPerspectiveTransform(card_corners, image_corners);
	warpPerspective(image_resized, image_perspective, perspective_kernel, image_resized.size());

	return image_perspective;
}

Mat PerspectiveTransformation(Mat image)
{
	Mat image_resized = TranslateCardImage(image);
	Mat image_binary;
	image_binary = Binarization(image_resized);

	vector<vector<Point> > contours = FindingContours(image_binary);
	vector<Point2f> card_corners = FindCardCorners(contours); //Source points for perspective transformation

	//Destination corners are corners of an image, they are sorted by distance from (0,0)
	vector<Point2f> image_corners =
	{ Point2f(0,0) , Point2f(image_resized.cols, 0), Point2f(0, image_resized.rows), Point2f(image_resized.cols, image_resized.rows) };

	if (image_resized.rows <= image_resized.cols)
	{
		image_corners = 
		{ Point2f(0,0) , Point2f(0, image_resized.rows), Point2f(image_resized.cols, 0), Point2f(image_resized.cols, image_resized.rows) };
	}
	SortVector(card_corners);
	Mat image_perspective = CreatePerspectiveImage(image_resized, image_corners, card_corners);

	return image_perspective;
}

void RecogniseCards()
{
	Mat image = OpenImage();
	if (EmptyImage(image))
	{
		MessageWrongFile();
		return;
	}
	DisplayImage(image);

	Mat image_resized = EnlargeImage(image);
	Mat image_filtered = ApplyFilters(image_resized);
	Mat image_binary;
	image_binary = Binarization(image_filtered);
	
	vector<vector<Point> > contours = FindingContours(image_binary);

	Rect* cards = new Rect[8]; 
	cards = FindCardsContours(contours);
	Card* detected_cards = new Card[4];

	for (int i = 0; i < 4; i++)
	{
		detected_cards[i].perspective_image = image_filtered(cards[2*i]); //Every card has 2 contours
		detected_cards[i].perspective_image = PerspectiveTransformation(detected_cards[i].perspective_image);

		Mat number_roi = PickROIForNumberRecognition(detected_cards[i].perspective_image);
		NumberRecognition(number_roi, detected_cards[i]);

		Mat symbol_roi = PickROIForSymbolRecognition(detected_cards[i].perspective_image);
		SymbolRecognition(symbol_roi, detected_cards[i]);

		DisplayRecognisedCard(detected_cards[i]);
	}

	MessageRecognisedCards(detected_cards);
	SortCards(detected_cards);
	DisplaySortedCards(detected_cards);

	waitKey(0);
	cvDestroyAllWindows();
	delete[] cards;
	delete[] detected_cards;
	image.release();
}


#include "cards_sorting.h"
#include "interface.h"
#include "numbers_recognition.h"


Mat OpenImage()
{
	string name = FileName();
	Mat image = imread(name);
	return image;
}

bool EmptyImage(Mat image)
{
	if (!image.data)
	{
		return true;
	}
	else
		return false;
}

Mat EnlargeImage(Mat image)
{
	Mat image_resized;
	image_resized = Mat::zeros(image.rows + 10, image.cols, image.type()); //More rows because some cards are partly cut off at the bottom
	Rect image_original(0, 0, image.cols, image.rows);
	image.copyTo(image_resized(image_original));
	return image_resized;
}

Mat MedianFilter(Mat image)
{
	Mat image_filtered;
	medianBlur(image, image_filtered, 5);
	return image_filtered;
}

Mat SharpenImage(Mat image)
{
	Mat image_blurred;
	Mat image_sharpened;
	GaussianBlur(image, image_blurred, Size(15, 15), 5, 5); //arbitrary parameters

	double alpha = 8; //constant value for unsharp mask filter
	addWeighted(image, 1 + alpha, image_blurred, -alpha, 0, image_sharpened);

	return image_sharpened;
}

Mat GammaCorrection(Mat image_filtered)
{
	double gamma = 0.65; //arbitrary value to deal with darker images

	Mat look_up_table(1, 256, CV_8U);
	uchar* tab = look_up_table.ptr();
	for (int i = 0; i < 256; ++i)
	{
		tab[i] = saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);
	}
	Mat image_LUT = image_filtered.clone();
	LUT(image_filtered, look_up_table, image_LUT);
	return image_LUT;
}

Mat ApplyFilters(Mat image)
{
	Mat image_filtered = MedianFilter(image);
	Mat image_sharpened = SharpenImage(image_filtered);
	Mat image_processed = GammaCorrection(image_sharpened);
	return image_processed;
}

Mat Binarization(Mat image)
{
	Mat image_binary;
	cvtColor(image, image, CV_BGR2GRAY);
	threshold(image, image_binary, 100, 255, CV_THRESH_OTSU);
	return image_binary;
}

Mat CannyEdgeDetection(Mat image_binary)
{
	Mat image_edges;
	Canny(image_binary, image_edges, 15, 255, 7);
	return image_edges;
}

void ClosingOperation(Mat &image)
{
	dilate(image, image, Mat());
	erode(image, image, Mat());
}
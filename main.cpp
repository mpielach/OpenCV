/*
Projekt wykonywany w ramach przedmiotu Widzenie Maszynowe.
Autor: Mateusz Pielach
Nr indeksu: 278231
Temat projektu: Projekt nr 2. Rozpoznawanie kart.
*/

#include <iostream>

#include "segmentation.h"
#include "interface.h"
#include "numbers_recognition.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

int main()
{
	RecogniseCards();
	return 0;
}
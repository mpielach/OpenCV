#include "interface.h"
#include "numbers_recognition.h"
#include "symbols_recognition.h"
#include "filtration.h"

string FileName()
{
	cout << "Please enter name of an image" << endl;
	string name;
	cin >> name;
	return name;
}

void MessageWrongFile()
{
	cout << "Can't open file" << endl;
}

void DisplayImage(Mat image)
{
	cvNamedWindow("Original image", WINDOW_NORMAL);
	imshow("Original image", image);
}

void MessageRecognisedCards(Card* detected_cards)
{
	cout << "Recognised cards are:" << endl;
	cout << "First: " << detected_cards[0].number << " of "<< detected_cards[0].symbol << endl;
	cout << "Second: " << detected_cards[1].number << " of " << detected_cards[1].symbol << endl;
	cout << "Third: " << detected_cards[2].number << " of " << detected_cards[2].symbol << endl;
	cout << "Fourth: " << detected_cards[3].number << " of " << detected_cards[3].symbol << endl;
}

void DisplayRecognisedCard(Card recognised_card)
{
	string number = to_string(recognised_card.number);
	string window_name = number + " of " + recognised_card.symbol;
	cvNamedWindow(window_name.c_str() , WINDOW_NORMAL);
	imshow(window_name, recognised_card.perspective_image);
}

void DisplaySortedCards(Card* detected_cards)
{
	cout << "Cards sorted in increasing order: " << endl;
	for (int i = 0; i < 4; i++)
	{
		cout << detected_cards[i].number << " of " << detected_cards[i].symbol << endl;
	}
}
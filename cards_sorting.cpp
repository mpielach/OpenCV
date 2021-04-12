#include "cards_sorting.h"
#include "interface.h"
#include "numbers_recognition.h"
#include "filtration.h"

bool SortCardsByNumber(Card card_a, Card card_b)
{
	return (card_a.number < card_b.number);
}

void SortCards(Card *&detected_cards)
{
	sort(detected_cards, detected_cards + 4, SortCardsByNumber);
}
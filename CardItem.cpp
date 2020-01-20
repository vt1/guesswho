#include "GameBoardComputer.h"
#include "GameBoard.h"
#include "CardParam.h"
#include "CardItem.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace std;

static void problemLoading(const char* filename) noexcept
{
	printf("Error while loading: %s\n", filename);
}

CardItem::CardItem() noexcept {}
CardItem::~CardItem() {}

CardItem* CardItem::create(const std::string& fileName, const std::string& name)
{
	CardItem *pCardItem = new CardItem();
	if (pCardItem->initWithFile(fileName))
    	{		
		pCardItem->setCardItemName(name);
		pCardItem->setCardItemFileName(fileName);
		pCardItem->isCardItemHidden = false;
        	pCardItem->autorelease();
		pCardItem->shouldBeHidden = false;
		pCardItem->isFadeActionRunning = false;				
	        pCardItem->initOptions();
        	return pCardItem;
    	}
    	CC_SAFE_DELETE(pCardItem);
   	return nullptr;
}

void CardItem::setCardItemName(const std::string& name)
{
	this->name = name;
}

#ifndef _CARD_ITEM_H_
#define _CARD_ITEM_H_

#include "CardParam.h"
#include "cocos2d.h"

using namespace std;

USING_NS_CC;

class CardItem : public cocos2d::Sprite
{
public:
	CardItem() noexcept;
    	~CardItem();
    	static CardItem* create(const std::string& fileName, const std::string& name);

	void setCardItemName(const std::string& name);
	string getCardItemName() const;
	void setCardItemFileName(const std::string& fileName);
	string getCardItemFileName() const;
	void setCardItemParameter(const std::string& parameter);
	string getCardItemParameter() const;
	void setCardItemImage(string fileName);

	bool isCardHidden() const noexcept;
	void setIsCardHidden(bool isCardItemHidden) noexcept;
	void setShouldBeHidden(bool hide) noexcept;
	bool getShouldBeHidden() const noexcept;

	void addCardParamItem(CardParam* pCardParam);
	CardParam* getCardParamItem(int index) const;
	vector<CardParam*> getCardParams() const;
	int getCardParamSize() const noexcept;
	
	void stopFadeActionForCard();
	void setIsFadeActionRunning(bool isFadeActionRunning) noexcept;
	bool getIsFadeActionRunning() const noexcept;

    	void initOptions();
	void onEnter();
	void onExit();
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
    	void touchEvent(cocos2d::Touch* touch, cocos2d::Vec2 _p);	
	
	void setSecredCardFromGameBoard();
	void setSecredCardFromGameBoardSecondPlayer();
	void switchToGameBoardComputer();
	void returnToGameBoard();
	
private:	
	std::string name;
	std::string fileName;
	std::string parameter;	
	vector<CardParam*> cardParams;	
		
	bool isCardItemHidden;	
	bool shouldBeHidden;	
	bool isFadeActionRunning;	
};

#endif
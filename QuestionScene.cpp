#include "QuestionScene.h"
#include "GameBoard.h"
#include "GameMenu.h"
#include "extensions/ExtensionMacros.h"
#include "QuestionPopUp.h"
#include "Template.h"
#include "GameBoardComputer.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocos2d;
using namespace cocos2d::extension;

void QuestionLayer::autoQuestionDeleting(bool isSecondPlayer)
{			
	int cntTrue = 0;
	int cntFalse = 0;		
	cocos2d::Vector<CardItem*> cardItemsTrueParam;
	cocos2d::Vector<CardItem*> cardItemsFalseParam;
	string keyQuestion;
	auto pGameBoardLayer = this->getGameBoardLayer();	
	for(int i = 0; i < this->questionsItems.size(); i++)
	{		
		bool allCardsTrueParamClosed = false;	
		bool allCardsFalseParamClosed = false;
		for(int j = 0; j < pGameBoardLayer->getCardItemsSize(); j++)
		{
			if(isSecondPlayer)
			{
				auto cardItem = pGameBoardLayer->getCardItem(j, pGameBoardLayer->getCardItemsComputerVector());	
				keyQuestion = cardItem->getCardParamItem(i)->key;
				string valueCardItem = cardItem->getCardParamItem(i)->value;

				if(valueCardItem == "true")
				{
					cardItemsTrueParam.pushBack(cardItem);
				}
				else
				{
					cardItemsFalseParam.pushBack(cardItem);
				}	
			}
			else
			{
				auto cardItem = pGameBoardLayer->getCardItem(j, pGameBoardLayer->getCardItemsVector());	
				keyQuestion = cardItem->getCardParamItem(i)->key;
				string valueCardItem = cardItem->getCardParamItem(i)->value;

				if(valueCardItem == "true")
				{
					cardItemsTrueParam.pushBack(cardItem);
				}
				else
				{
					cardItemsFalseParam.pushBack(cardItem);
				}	
			}
		}	

		for(int g = 0; g < cardItemsTrueParam.size(); g++)
		{
			if(cardItemsTrueParam.at(g)->isCardHidden())
			{
				cntTrue++;
			}
		}

		if(cardItemsTrueParam.size() == cntTrue)
		{
			allCardsTrueParamClosed = true;			
		}

		for(int g = 0; g < cardItemsFalseParam.size(); g++)
		{
			if(cardItemsFalseParam.at(g)->isCardHidden())
			{
				cntFalse++;
			}
		}

		if(cardItemsFalseParam.size() == cntFalse)
		{
			allCardsFalseParamClosed = true;			
		}
				
		if(allCardsFalseParamClosed || allCardsTrueParamClosed)
		{		
			for(int h = 0; h < questionsItems.size(); h++)
			{
				if(questionsItems.at(h)->getKeyQuestion() == keyQuestion)
				{					
					questionsItems.at(h)->removeFromParent();
					questionsItems.at(h)->setQuestionDeleted(true);
					keyQuestion = "";							
				}	
				else
				{
					//
				}
			}		
		}
		cardItemsTrueParam.clear();
		cardItemsFalseParam.clear();		
		cntTrue = 0;
		cntFalse = 0;
	}	

	pGameBoardLayer->clearQuestionsVector();
	if(isSecondPlayer)
	{
		for(int y = 0; y < questionsItems.size(); y++)
		{
			if(!questionsItems.at(y)->isQuestionDeleted())
			{
				pGameBoardLayer->addIndexOpenQuestionSecPlayer(y);
			}
		}
	}
	else
	{
		for(int y = 0; y < questionsItems.size(); y++)
		{
			if(!questionsItems.at(y)->isQuestionDeleted())
			{
				pGameBoardLayer->addIndexOpenQuestion(y);
			}
		}
	}
	updateQuestionButtonPosition(isSecondPlayer);
}
                                        
void QuestionLayer::questionLogic(QuestionItem *pQuestionItem)
{		                                                                
	auto* pNode = dynamic_cast<GameBoardComputerScene*>(this->getParent());	
	if(pNode != NULL)
	{	
		auto *pGameBoardComputerScene = static_cast<GameBoardComputerScene*>(this->getParent());
		GameBoardLayer *pGameBoardLayer = pGameBoardComputerScene->getLayer()->getGameBoardLayer();
		
		int iterator = 0;		
		int numberOfParams = pGameBoardLayer->getCardsReceiver()->getCardParamSize();	

		string keyQuest = pQuestionItem->getKeyQuestion();
		string question = pQuestionItem->getQuestion();
		const char* charKeyQuest = keyQuest.c_str();	
		for(int i = 0; i < numberOfParams; i++) //get iterator of key question
		{
			string key = pGameBoardLayer->getCardsReceiver()->getCardParam(i)->key;
			const char *charKey = key.c_str();
			if(strcmp(charKey, charKeyQuest) == 0)
			{
				iterator = i;
				break;	
			}
		}
		cocos2d::Vector<CardItem*> cards;
		for (int j = 0; j < pGameBoardLayer->getCardItemsComputerSize(); j++)
		{
			auto cardItem = pGameBoardLayer->getCardItem(j, pGameBoardLayer->getCardItemsComputerVector());
			int numberOfParams = cardItem->getCardParamSize();
			string keyQuest = pQuestionItem->getKeyQuestion();
			const char* charKeyQuest = keyQuest.c_str();
			for (int i = 0; i < numberOfParams; i++)
			{
				string key = cardItem->getCardParamItem(i)->key;
				const char *charKey = key.c_str();
				if (strcmp(charKey, charKeyQuest) == 0)
				{
					if (cardItem->getCardParamItem(i)->value == "true")
					{
						cards.pushBack(cardItem);
					}
				}
			}
		}
		srand(time(NULL));
		int randIndex = rand() % cards.size();
		auto card = cards.at(randIndex);
		pGameBoardLayer->createQuestionPopUp(question, iterator, false, true, card);
	}
	else
	{
		cocos2d::Vector<CardItem*> cards;
		auto pGameBoardLayer = this->getGameBoardLayer();
		int iterator = 0;
		int numberOfParams = pGameBoardLayer->getCardsReceiverComputer()->getCardParamSize();	

		string keyQuest = pQuestionItem->getKeyQuestion();
		string question = pQuestionItem->getQuestion();
		const char* charKeyQuest = keyQuest.c_str();	
		for(int i = 0; i < numberOfParams; i++) //get iterator of key question
		{
			string key = pGameBoardLayer->getCardsReceiverComputer()->getCardParam(i)->key;
			const char *charKey = key.c_str();
			if(strcmp(charKey, charKeyQuest) == 0)
			{
				iterator = i;
				break;	
			}
		}	
		for (int j = 0; j < pGameBoardLayer->getCardItemsSize(); j++)
		{
			auto cardItem = pGameBoardLayer->getCardItem(j, pGameBoardLayer->getCardItemsVector());
			int numberOfParams = cardItem->getCardParamSize();
			string keyQuest = pQuestionItem->getKeyQuestion();
			const char* charKeyQuest = keyQuest.c_str();
			for (int i = 0; i < numberOfParams; i++)
			{
				string key = cardItem->getCardParamItem(i)->key;
				const char *charKey = key.c_str();
				if (strcmp(charKey, charKeyQuest) == 0)
				{
					if(cardItem->getCardParamItem(i)->value == "true")
					{
						cards.pushBack(cardItem);
					}
				}
			}
		}
		srand(time(NULL));
		int randIndex = rand() % cards.size();
		auto card = cards.at(randIndex);
		pGameBoardLayer->createQuestionPopUp(question, iterator, false, false, card);
	}	
}

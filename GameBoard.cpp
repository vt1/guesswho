#include "GameBoard.h"
#include "GameBoardComputer.h"
#include "QuestionPopUp.h"
#include "GuessPopUp.h"
#include "SimpleAudioEngine.h"
 
#ifdef SDKBOX_ENABLED
#include "PluginAdMob/PluginAdMob.h"
#endif
using namespace std;
using namespace cocos2d;

void GameBoardLayer::createCards(Vector<CardItem*> *cardItemVector)
{	
	string line;
	string name;
	string image;	
	int cnt = 0;

	ssize_t pSize = 0;
	unsigned char *str = FileUtils::getInstance()->getFileData("character_nameAndImage.txt", "rb",  &pSize);	
	cocos2d::CCString *ccStr = cocos2d::CCString::createWithData(str, pSize);
	const char* chStr = ccStr->getCString();
	istringstream in(chStr);
	
	while (std::getline(in, line)) 
	{		
		line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
		line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
		int strLength = line.size();		
		char *chStr = new char[strLength+1];
		strcpy(chStr, line.c_str());
		char *word;		
		word = strtok(chStr,":;,.-");
		name = word;			
		word = strtok(NULL,":;,-");
		image = word;	
		//word = strtok(NULL,":;,-");	
		CardItem* pCardItem = CardItem::create(image,name);	
		if(guessCardImages.size() != 24)
		{
			Sprite *pGuessImage = Sprite::create("guessImage.png");				
			this->addGuessImage(pGuessImage);
		}
		if(hiddenCardImages.size() != 24)
		{
			Sprite *pHiddenImage = Sprite::create("hidden.png");
			pHiddenImage->setTag(2);
			this->addHiddenImage(pHiddenImage);
		}		


		this->addCardItem(pCardItem, cardItemVector);	
		delete[] chStr;
	}		
}

void GameBoardLayer::createInfoPopUp(Layer *pLayer)
{
	pInfoPopUp = InfoPopUpLayer::create();
	auto gameBoardScene = this->getParent();
	pLayer->addChild(pInfoPopUp, 2);
}

void GameBoardLayer::makeMove() // comp
{
	int moveOrGuess = 2; // 1 - move; 0 - guess;
	int resultMoveOrGuess = rand()% moveOrGuess;
	int numberOfCards = this->getCardItemsComputerSize();
	auto callBackReturnToGameBoard = CallFunc::create(CC_CALLBACK_0(GameBoardLayer::returnToGameBoard, this));	
	auto callBackChangeGuessCardImg = CallFunc::create(CC_CALLBACK_0(GameBoardLayer::changeGuessPopUpImage, this));	
	auto callBackHideGuessPopUp = CallFunc::create(CC_CALLBACK_0(GameBoardLayer::hideGuessPopUpComputer, this));
	auto callBackCreateGameOverPopUpComp = CallFunc::create(CC_CALLBACK_0(GameBoardLayer::createGameOverPopUp, this));
	//we have an array "iterators". it contains iterators those cards, which aren't hidden because computer guesses only opened cards
	for(int i = 0; i < cardItemsComputer.size(); i++) // f cards iterators which are not hidden
	{
		if(!cardItemsComputer.at(i)->isCardHidden())		
		{
			iteratorsOpenCards.push_back(i);
		}
	}

	int numberOfQuestions = this->questItems.size();
	int randomQuest = rand() % numberOfQuestions;
	cocos2d::Vector<CardItem*> cards;

	if(iteratorsOpenCards.size() == 1) // guess
	{
		pGuessPopUpLayer = GuessPopUpLayer::create();			
		pGuessPopUpLayer->createCardItemImage(getCardItem(iteratorsOpenCards.at(0), cardItemsComputer)->getCardItemFileName());
		pGameBoardComputerScene->addChild(pGuessPopUpLayer);
		//string hiddenCard = getCardItem(iteratorsOpenCards.at(0), cardItemsComputer)->getCardItemHiddenImage();
		//getCardItem(iteratorsOpenCards.at(0), cardItemsComputer)->setCardItemImage(hiddenCard);
		pGuessPopUpLayer->setIsGuessCorrectComputer(true);
		isPlayerWin = false;
		auto action = Sequence::create(DelayTime::create(1), callBackChangeGuessCardImg,
												DelayTime::create(2), callBackHideGuessPopUp,
													DelayTime::create(2), callBackCreateGameOverPopUpComp ,nullptr);
		pGameBoardComputerScene->getLayer()->runAction(action);	
	}
	else
	{
		if(resultMoveOrGuess == 1) // 1 - move; 0 - guess;
		{
			int iterator = 0;		
			int numberOfParams = this->getCardsReceiver()->getCardParamSize();			

			string keyQuest = getQuestionItem(randomQuest)->getKeyQuestion();
			const char* charKeyQuest = keyQuest.c_str();	
			string question = getQuestionItem(randomQuest)->getQuestion();
			for(int i = 0; i < numberOfParams; i++) //get iterator of key question
			{
				string key = this->getCardsReceiver()->getCardParam(i)->key;
				const char *charKey = key.c_str();
				if(strcmp(charKey, charKeyQuest) == 0)
				{
					iterator = i;
					break;
				}
			}	
			for (int j = 0; j < getCardItemsComputerSize(); j++)
			{
				auto cardItem = getCardItem(j, getCardItemsComputerVector());
				int numberOfParams = cardItem->getCardParamSize();
				string keyQuest = getQuestionItem(randomQuest)->getKeyQuestion();
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
			createQuestionPopUp(question, iterator, true, false, card);
			auto callBackChangeImage = CallFunc::create(CC_CALLBACK_0(GameBoardLayer::changePopUpImageComputer, this));	
			auto callBackHidePopUp = CallFunc::create(CC_CALLBACK_0(GameBoardLayer::hideQuestionPopUpComputer, this));	 
			auto action = Sequence::create(DelayTime::create(1), callBackChangeImage,
											DelayTime::create(1), callBackHidePopUp,
											DelayTime::create(1), callBackReturnToGameBoard ,nullptr);
			pGameBoardComputerScene->getLayer()->runAction(action);	
			for(int i = 0; i < numberOfCards; i++) //hide cards which dont fit us
			{
				string valueCardsComputerReceiver = this->getCardsReceiver()->getCardParam(iterator)->value;
				string valueCardItem = this->getCardItem(i, cardItemsComputer)->getCardParamItem(iterator)->value;
				if(valueCardsComputerReceiver != valueCardItem)
				{
					if(valueCardItem != "missing")
					{						
						this->getCardItem(i, cardItemsComputer)->setIsCardHidden(true);
					}
				}
			}	
			questItems.erase(randomQuest);
			iteratorsOpenCards.clear();
		}
		else
		{   // guess	
			int lengthIteratorsArr = iteratorsOpenCards.size();
			int randomCard = rand() % lengthIteratorsArr;

			pGuessPopUpLayer = GuessPopUpLayer::create();			
			pGuessPopUpLayer->createCardItemImage(getCardItem(iteratorsOpenCards.at(randomCard), cardItemsComputer)->getCardItemFileName());
			pGameBoardComputerScene->addChild(pGuessPopUpLayer);				

			bool isGuessCorrect = getCardItem(iteratorsOpenCards.at(randomCard), cardItemsComputer)->getCardItemName() 
												== pCardsReceiver->getSecretCardName();
			if(isGuessCorrect)
			{
				//string hiddenCard = getCardItem(iteratorsOpenCards.at(randomCard), cardItemsComputer)->getCardItemHiddenImage();
				//getCardItem(iteratorsOpenCards.at(randomCard), cardItemsComputer)->setCardItemImage(hiddenCard);
				pGuessPopUpLayer->setIsGuessCorrectComputer(true);
				isPlayerWin = false;
				auto action = Sequence::create(DelayTime::create(1), callBackChangeGuessCardImg,
												DelayTime::create(2), callBackHideGuessPopUp,
													DelayTime::create(2), callBackCreateGameOverPopUpComp ,nullptr);
				pGameBoardComputerScene->getLayer()->runAction(action);	
			}
			else
			{
				pGuessPopUpLayer->setIsGuessCorrectComputer(false);		
				auto action = Sequence::create(DelayTime::create(1), callBackChangeGuessCardImg,
												DelayTime::create(1), callBackHideGuessPopUp,
													DelayTime::create(1), callBackReturnToGameBoard ,nullptr);
				pGameBoardComputerScene->getLayer()->runAction(action);	
				//string hiddenCard = getCardItem(iteratorsOpenCards.at(randomCard), cardItemsComputer)->getCardItemHiddenImage();
				//getCardItem(iteratorsOpenCards.at(randomCard), cardItemsComputer)->setCardItemImage(hiddenCard);
				//getCardItem(iteratorsOpenCards.at(randomCard), cardItemsComputer)->setAnchorPoint(Vec2(0, 0));
				getCardItem(iteratorsOpenCards.at(randomCard), cardItemsComputer)->setIsCardHidden(true);
			}	
			iteratorsOpenCards.clear();
		}
	}
}
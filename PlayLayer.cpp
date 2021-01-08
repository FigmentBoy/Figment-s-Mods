#include "PlayLayer.h"
#include "MinHook.h"
#include <Windows.h>
#include "GameManager.h"

void __fastcall PlayLayer::togglePracticeHook(CCLayer* self, int edx, bool practice) {
	void* gm = GameManager::getSharedState();
	if (practice) {
		if (GameManager::getGameVariable(gm, "1001")) { // In Practice
			CCLabelBMFont* textNode = CCLabelBMFont::create("In Practice", "Resources\\bigFont-uhd.fnt");
			auto window = CCDirector::sharedDirector()->getWinSize();

			textNode->setScale(0.5f);
			textNode->setPosition({ window.width - textNode->getScaledContentSize().width / 2, window.height - textNode->getScaledContentSize().height / 2 });
			textNode->setTag(1001);
			self->addChild(textNode, 5);
		}
	}
	else {
		self->removeChildByTag(1001);
	}

	
	togglePractice(self, practice);
}

bool __fastcall PlayLayer::initHook(CCLayer* self, int edx, void* GJGameLevel) {
	void* gm = GameManager::getSharedState();
	if (GameManager::getGameVariable(gm, "1000")) { // Level Info
		int levelID = *reinterpret_cast<int*>((uintptr_t)GJGameLevel + 0xF8);
		char* levelName = reinterpret_cast<char*>((uintptr_t)GJGameLevel + 0xFC);
		char* levelAuthor = reinterpret_cast<char*>((uintptr_t)GJGameLevel + 0x144);

		std::string text;
		if (std::string(levelAuthor) == "") {
			text = std::string(levelName) + " by RobTop\nLevel ID: " + std::to_string(levelID);
		}
		else if (levelID == 0) {
			text = std::string(levelName) + " by " + std::string(levelAuthor) + "\n(Not Published)";
		}
		else {
			text = std::string(levelName) + " by " + std::string(levelAuthor) + "\nLevel ID: " + std::to_string(levelID);
		}


		CCLabelBMFont* textNode = CCLabelBMFont::create(text.c_str(), "Resources\\bigFont-uhd.fnt", 0.0f, kCCTextAlignmentCenter);
		auto window = CCDirector::sharedDirector()->getWinSize();

		textNode->setScale(0.5f);
		textNode->setPosition({ window.width / 2, 30 });
		self->addChild(textNode, 5);
	}
	return init(self, GJGameLevel);
}


void PlayLayer::mem_init() {
	size_t base = reinterpret_cast<size_t>(GetModuleHandle(0));
	MH_CreateHook(
		(PVOID)(base + 0x01FB780),
		PlayLayer::initHook,
		(LPVOID*)&PlayLayer::init);

	MH_CreateHook(
		(PVOID)(base + 0x20D0D0),
		PlayLayer::togglePracticeHook,
		(LPVOID*)&PlayLayer::togglePractice);
}


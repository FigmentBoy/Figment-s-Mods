#include "MainLayer.h"
#include "MinHook.h"
#include <Windows.h>
#include "GameManager.h"


bool __fastcall MainLayer::initHook(CCLayer* self, int edx) {
	void* gm = GameManager::getSharedState();
	if (!GameManager::getGameVariable(gm, "1002")) { // Hide Watermark
		CCLabelBMFont* textNode = CCLabelBMFont::create("Mods by Figment", "Resources\\goldFont-uhd.fnt");
		auto window = CCDirector::sharedDirector()->getWinSize();

		textNode->setPosition({ window.width / 2, window.height * 3 / 4 });

		self->addChild(textNode, 5);
	}
	return init(self);

}


void MainLayer::mem_init() {
	size_t base = reinterpret_cast<size_t>(GetModuleHandle(0));
	MH_CreateHook(
		(PVOID)(base + 0x1907b0),
		MainLayer::initHook,
		(LPVOID*)&MainLayer::init);
}
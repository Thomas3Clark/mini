#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "Adventure.h"
#include "Battle.h"
#include "Character.h"
#include "Items.h"
#include "MainMenu.h"
#include "Menu.h"
#include "Shop.h"
#include "UILayers.h"
#include "Utils.h"

const char *UpdateFloorText(void)
{
	static char floorText[] = "00"; // Needs to be static because it's used by the system later.
	IntToString(floorText, 2, GetCurrentFloor());
	return floorText;
}

int8_t updateDelay = 0;
bool adventureWindowVisible = false;

void AdventureWindowAppear(Window *window);
void AdventureWindowDisappear(Window *window);

#if ALLOW_TEST_MENU
void ShowDebugMenu(void);
#endif

MenuDefinition adventureMenuDef = 
{
	.menuEntries = 
	{
		{"Main", "Open the main menu", ShowMainMenu},
#if ALLOW_TEST_MENU
		{NULL, NULL, NULL},
		{"Debug", "Variables values", ShowDebugMenu},
		{NULL, NULL, NULL},
		{NULL, NULL, NULL},
		{"", "", ShowTestMenu}
#endif
	},
	.appear = AdventureWindowAppear,
	.disappear = AdventureWindowDisappear,
	.animated = true,
	.mainImageId = RESOURCE_ID_IMAGE_DUNGEONRIGHT
};

Window *adventureWindow = NULL;

void LoadRandomDungeonImage(void)
{
#if ALLOW_RANDOM_DUNGEON_GRAPHICS		
	int result;
#endif
	
	if(!adventureWindow)
		return;

#if ALLOW_RANDOM_DUNGEON_GRAPHICS		
	result = Random(12);
	if(result < 6)
		adventureMenuDef.mainImageId = RESOURCE_ID_IMAGE_DUNGEONSTRAIGHT;
	else if(result < 9)
		adventureMenuDef.mainImageId = RESOURCE_ID_IMAGE_DUNGEONLEFT;
	else if(result < 12)
		adventureMenuDef.mainImageId = RESOURCE_ID_IMAGE_DUNGEONRIGHT;
	else
		adventureMenuDef.mainImageId = RESOURCE_ID_IMAGE_DUNGEONDEADEND;
#endif

	LoadMainBmpImage(adventureWindow, adventureMenuDef.mainImageId);
}

void AdventureWindowAppear(Window *window)
{
	MenuAppear(window);
	ShowMainWindowRow(0, "Floor", UpdateFloorText());
	adventureWindow = window;
	UpdateCharacterHealth();
	UpdateCharacterLevel();
	updateDelay = 1;
	adventureWindowVisible = true;
}

void AdventureWindowDisappear(Window *window)
{
	adventureWindowVisible = false;
	MenuDisappear(window);
	adventureWindow = NULL;
}

void ShowAdventureWindow(void)
{
	PushNewMenu(&adventureMenuDef);
}

#if ALLOW_SHOP
// These should add up to 100
static CardDeck entries[] = 
{
	{ShowItemGainWindow, 15,0,"Item"},
	{ShowBattleWindow, 12,0,"Battle"},
	{ShowNewFloorWindow, 4,0,"Floor"},
	{ShowShopWindow, 2,0,"Shop"}
};
static uint8_t entriesSize = 4;
static uint8_t limitGetCard = 4;
#else
// These should add up to 100
static CardDeck entries[] = 
{
	{ShowItemGainWindow, 12,0,"Item"},
	{ShowBattleWindow, 16,0,"Battle"},
	{ShowNewFloorWindow, 4,0,"Floor"}
};
static uint8_t entriesSize = 3;
static uint8_t limitGetCard = 3;
#endif


#if EVENT_CHANCE_SCALING
static uint16_t ticksSinceLastEvent = 0;
#endif

void SwapCard(uint8_t i,uint8_t j) {
	CardDeck temp = entries[i];
	entries[i] = entries[j];
	entries[j] = temp;
}

void ResetCurrentTaken() {
	limitGetCard = entriesSize;
	for(uint8_t i = 0; i < entriesSize; ++i) {
		entries[i].current = 0;
	}
}
CardDeck *GetCard() {
	CardDeck *cd;
	uint16_t toTake;
	if(limitGetCard == 1) {
		toTake = 0;
	}
	else {
		toTake = Random(limitGetCard)-1;
	}

	cd = &entries[toTake];
	cd->current++;
	if(cd->current == cd->number) {
		if(limitGetCard == 1)
			ResetCurrentTaken();
		else {
			if(limitGetCard-1 != toTake)
				SwapCard(toTake,limitGetCard-1);
			limitGetCard--;
		}
	}
	return cd;
	
}
bool ComputeRandomEvent(bool fastMode)
{
	uint16_t result = Random(100);
	uint16_t chanceOfEvent = EVENT_CHANCE_BASE;
#if EVENT_CHANCE_SCALING
	if(ticksSinceLastEvent > 5)
	{
		chanceOfEvent += (ticksSinceLastEvent - 2) * 3;
	}
#endif
	
	if(!fastMode && result > chanceOfEvent)
		return false;
		

	if(GetVibration())
		vibes_short_pulse();
		
	CardDeck *cd = GetCard();
	cd->windowFunction();
	
#if EVENT_CHANCE_SCALING
		ticksSinceLastEvent = 0;
#endif
	return true;
}

void UpdateAdventure(void)
{
	if(!adventureWindowVisible)
		return;
#if EVENT_CHANCE_SCALING
	++ticksSinceLastEvent;
#endif
	if(updateDelay && !GetFastMode())
	{
		--updateDelay;
		return;
	}
 
	ComputeRandomEvent(GetFastMode());
	LoadRandomDungeonImage();
}

void NewFloorMenuInit(Window *window);
void NewFloorMenuAppear(Window *window);
void ContinueNextFloor(void);
void CheckEasyMode(MenuEntry *menuEntries);

MenuDefinition newFloorMenuDef = 
{
	.menuEntries = 
	{
		{"Go", "Continue adventuring", ContinueNextFloor}
	},
	.modify = CheckEasyMode,
	.init = NewFloorMenuInit,
	.appear = NewFloorMenuAppear,
	.mainImageId = RESOURCE_ID_IMAGE_NEWFLOOR
};

void NewFloorMenuInit(Window *window)
{
	MenuInit(window);
}

void ContinueNextFloor() {
	IncrementFloor();
	PopMenu();
}

void NewFloorMenuAppear(Window *window)
{
	MenuAppear(window);
	ShowMainWindowRow(0, "New Floor", UpdateFloorText());
}

void ShowNewFloorWindow(void)
{
	PushNewMenu(&newFloorMenuDef);
}

void CheckEasyMode(MenuEntry * menuEntries) {
	if(!GetEasyMode())
		return;
	MenuEntry stay = {"Stay", "Stay in the same floor", PopMenu};
	menuEntries[1] = stay;
}

#if ALLOW_TEST_MENU

const char *UpdateEntry0Text()
{
	static char entry0[] = "00"; // Needs to be static because it's used by the system later.
	IntToString(entry0, 2, entries[0].current);
	return entry0;
}

const char *UpdateEntry1Text()
{
	static char entry1[] = "00"; // Needs to be static because it's used by the system later.
	IntToString(entry1, 2, entries[1].current);
	return entry1;
}

const char *UpdateEntry2Text()
{
	static char entry2[] = "00"; // Needs to be static because it's used by the system later.
	IntToString(entry2, 2, entries[2].current);
	return entry2;
}

#if ALLOW_SHOP
const char *UpdateEntry3Text()
{
	static char entry3[] = "00"; // Needs to be static because it's used by the system later.
	IntToString(entry3, 2, entries[3].current);
	return entry3;
}
#endif

const char *UpdateLimitText()
{
	static char limitCard[] = "00"; // Needs to be static because it's used by the system later.
	IntToString(limitCard, 2, limitGetCard);
	return limitCard;
}


void DebugMenuAppear(Window *window)
{
	MenuAppear(window);
	uint8_t i=0;
	ShowMainWindowRow(i++, "Debug", "");	
	ShowMainWindowRow(i++, entries[0].name, UpdateEntry0Text());
	ShowMainWindowRow(i++, entries[1].name, UpdateEntry1Text());
	ShowMainWindowRow(i++, entries[2].name, UpdateEntry2Text());
#if ALLOW_SHOP
	ShowMainWindowRow(i++, entries[3].name, UpdateEntry3Text());
#endif
	ShowMainWindowRow(i++, "Active cards", UpdateLimitText());
}

MenuDefinition debugMenuDef = 
{
	.menuEntries = 
	{
		{"Quit", "Return to main menu", PopMenu},
	},
	.appear = DebugMenuAppear,
	.mainImageId = -1
};


void ShowDebugMenu(void)
{
	PushNewMenu(&debugMenuDef);
}
#endif

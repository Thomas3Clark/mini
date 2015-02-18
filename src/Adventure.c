#include "pebble.h"

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
	UIntToString(floorText,GetCurrentFloor());
	return floorText;
}

static int8_t updateDelay = 0;
static bool adventureWindowVisible = false;

void AdventureWindowAppear(Window *window);
void AdventureWindowDisappear(Window *window);

#if ALLOW_TEST_MENU
void ShowDebugMenu(void);
#endif

static MenuDefinition adventureMenuDef = 
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

static Window *adventureWindow = NULL;

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
	APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "Adventure appear floor %d",GetCurrentFloor());
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
static Card entries[] = 
{
	{ShowItemGainWindow, ITEM_CARDS,"Item"},
	{ShowBattleWindow, BATTLE_CARDS,"Battle"},
	{ShowNewFloorWindow, FLOOR_CARDS,"Floor"},
	{ShowShopWindow, SHOP_CARDS,"Shop"}
};
static uint8_t entriesSize = 4;
#else
static Card entries[] = 
{
	{ShowItemGainWindow, ITEM_CARDS,"Item"},
	{ShowBattleWindow, BATTLE_CARDS,"Battle"},
	{ShowNewFloorWindow, FLOOR_CARDS,"Floor"},
};
static uint8_t entriesSize = 3;
#endif
static uint8_t cardTaken = 0;

#if EVENT_CHANCE_SCALING
static uint8_t ticksSinceLastEvent = 0;
#endif

static Card* cardDeck[NB_CARDS];


void ShuffleDeck() {
	for(uint8_t i = 0; i < SHUFFLE_DECK_TIMES; i++) {
		uint8_t first = Random(NB_CARDS);
		uint8_t second = Random(NB_CARDS);
		if(first == second) {
			first++;
			first %= NB_CARDS;
		}
		Card* temp = cardDeck[first];
		cardDeck[first] = cardDeck[second];
		cardDeck[second] = temp;
	}
}

void ResetCurrentTaken() {
	cardTaken = 0;
	uint8_t index = 0;
	for(uint8_t i = 0; i < entriesSize; ++i) {
		for(uint8_t j = 0; j < entries[i].number; j++) {
			cardDeck[index++] = &entries[i];
		}
	}
	ShuffleDeck();
}

Card *GetCard() {
	cardTaken++;
	if(cardTaken == NB_CARDS) {
		ResetCurrentTaken();
	}
	return cardDeck[cardTaken];
}
bool ComputeRandomEvent(bool fastMode)
{
	uint16_t chanceOfEvent = EVENT_CHANCE_BASE;
#if EVENT_CHANCE_SCALING
	if (!ticksSinceLastEvent %2) {
		fastMode = true;
	} else if(ticksSinceLastEvent > 5) {
		chanceOfEvent += (ticksSinceLastEvent - 2) * 3;
	}
#endif
	if(!fastMode) {
		uint16_t result = Random(100);
		if(result > chanceOfEvent)
			return false;
	}
	
	if(GetVibration())
		vibes_short_pulse();
		
	Card *cd = GetCard();
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

static MenuDefinition newFloorMenuDef = 
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
	UIntToString(entry0, entries[0].number);
	return entry0;
}

const char *UpdateEntry1Text()
{
	static char entry1[] = "00"; // Needs to be static because it's used by the system later.
	UIntToString(entry1, entries[1].number);
	return entry1;
}

const char *UpdateEntry2Text()
{
	static char entry2[] = "00"; // Needs to be static because it's used by the system later.
	UIntToString(entry2, entries[2].number);
	return entry2;
}

#if ALLOW_SHOP
const char *UpdateEntry3Text()
{
	static char entry3[] = "00"; // Needs to be static because it's used by the system later.
	UIntToString(entry3, entries[3].number);
	return entry3;
}
#endif

const char *UpdateLimitText()
{
	static char limitCard[] = "00"; // Needs to be static because it's used by the system later.
	UIntToString(limitCard, cardTaken);
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
	ShowMainWindowRow(i++, "Taken", UpdateLimitText());
}

static MenuDefinition debugMenuDef = 
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

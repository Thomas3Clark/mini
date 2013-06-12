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

uint8_t currentCardIndex = 0;

static ShowWindowFunction baseDeck[32];

void AdventureWindowAppear(Window *window);
void AdventureWindowDisappear(Window *window);

MenuDefinition adventureMenuDef = 
{
	.menuEntries = 
	{
		{"Main", "Open the main menu", ShowMainMenu},
#if ALLOW_TEST_MENU
		{NULL, NULL, NULL},
		{NULL, NULL, NULL},
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
CardDeck entries[] = 
{
	{ShowItemGainWindow, 14},
	{ShowBattleWindow, 14},
	{ShowNewFloorWindow, 3},
	{ShowShopWindow, 1}
};
#else
// These should add up to 100
CardDeck entries[] = 
{
	{ShowItemGainWindow, 12},
	{ShowBattleWindow, 16},
	{ShowNewFloorWindow, 4}
};
#endif

#if EVENT_CHANCE_SCALING
static uint16_t ticksSinceLastEvent = 0;
#endif

void GenerateDeck() {
	uint8_t nbCards = 0;
	currentCardIndex = 0;
	for(size_t i = 0; i < sizeof(entries); ++i) {
		CardDeck *cd = &entries[i];
		for(uint8_t j = 0; j < cd->number; ++j) {
			baseDeck[nbCards++] = cd->windowFunction;
		}
	}
	for(uint8_t i = 0; i < nbCards-1; ++i) {
		uint8_t r = i + (Random(nbCards-i)-1);
		ShowWindowFunction temp = baseDeck[i]; 
		baseDeck[i] = baseDeck[r]; 
		baseDeck[r] = temp;
	}
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
		
	if(currentCardIndex == sizeof(baseDeck))
		GenerateDeck();
		
	if(GetVibration())
		vibes_short_pulse();
		
	if(baseDeck[currentCardIndex])
		baseDeck[currentCardIndex++]();
		
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

MenuDefinition newFloorMenuDef = 
{
	.menuEntries = 
	{
		{"Go", "Continue adventuring", ContinueNextFloor},
		{"Stay", "Stay in the same floor", PopMenu}
	},
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

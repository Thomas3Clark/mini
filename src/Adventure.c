#include "pebble.h"

#include "Adventure.h"
#include "Battle.h"
#include "Character.h"
#include "Items.h"
#include "Logging.h"
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
	
#if ALLOW_RANDOM_DUNGEON_GRAPHICS		
	result = Random(12) + 1;
	if(result < 6)
		adventureMenuDef.mainImageId = RESOURCE_ID_IMAGE_DUNGEONSTRAIGHT;
	else if(result < 9)
		adventureMenuDef.mainImageId = RESOURCE_ID_IMAGE_DUNGEONLEFT;
	else if(result < 12)
		adventureMenuDef.mainImageId = RESOURCE_ID_IMAGE_DUNGEONRIGHT;
	else
		adventureMenuDef.mainImageId = RESOURCE_ID_IMAGE_DUNGEONDEADEND;
#endif

	if(adventureWindow)
		LoadMainBmpImage(adventureWindow, adventureMenuDef.mainImageId);
}

void AdventureWindowAppear(Window *window)
{
	INFO_LOG("Back to the adventure.");
	DEBUG_LOG("Adventure appear floor %d",GetCurrentFloor());
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
	INFO_LOG("Adventure Window");
	PushNewMenu(&adventureMenuDef);
}

#if ALLOW_SHOP
static Card entries[] = 
{
	{ShowItemGainWindow, ITEM_CARDS,0,"Item"},
	{ShowBattleWindow, BATTLE_CARDS,0,"Battle"},
	{ShowNewFloorWindow, FLOOR_CARDS,0,"Floor"},
	{ShowShopWindow, SHOP_CARDS,0,"Shop"}
};

#else
static Card entries[] = 
{
	{ShowItemGainWindow, ITEM_CARDS,0,"Item"},
	{ShowBattleWindow, BATTLE_CARDS,0,"Battle"},
	{ShowNewFloorWindow, FLOOR_CARDS,0,"Floor"},
};
#endif
static uint8_t entriesSize =  sizeof(entries)/sizeof(entries[0]);

#if EVENT_CHANCE_SCALING
static uint8_t ticksSinceLastEvent = 0;
#endif

void SwapCardEntry(int first, int second) {
	Card temp = entries[first];
	entries[first] = entries[second];
	entries[second] = temp;
	DEBUG_LOG( "Swap: %d -> %d",first,second);
}

void ResetCurrentTaken() {
	entriesSize =  sizeof(entries)/sizeof(entries[0]);
	for(uint8_t i = 0; i < entriesSize; ++i) {
		(&entries[i])->taken = 0;
	}
}

void ComputeAdventure() {
	
	
	uint16_t rand = 0;
	if(entriesSize != 1) {
		rand = Random(entriesSize);
	}

	Card* card = &entries[rand];
	DEBUG_LOG( "Card Taken: %s",card->name);	
	
	card->windowFunction();	
	card->taken += 1;	
	if(card->taken == card->total) {
		entriesSize--;
		card->taken = 0;
		if(entriesSize == 0) {
			entriesSize = sizeof(entries)/sizeof(entries[0]);
			DEBUG_LOG( "Reset EntriesSize: %d",entriesSize);
		}
		else if(rand != entriesSize) {
			SwapCardEntry(rand, entriesSize);
		}
	}	
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
		
	ComputeAdventure();
	
#if EVENT_CHANCE_SCALING
		ticksSinceLastEvent = 0;
#endif
	return true;
}

void UpdateAdventure(void)
{
	if(!adventureWindowVisible)
		return;
	
	if(IsBattleForced())
	{
		INFO_LOG("Triggering forced battle.");
		ShowBattleWindow();
		return;
	}

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
const char * TextEntry0() {
	static char entry0[] = "00";
	strcpy(entry0, GenerateText(entries[0].taken));	
	return entry0;
}
const char * TextEntry1() {
	static char entry1[] = "00";
	strcpy(entry1, GenerateText(entries[1].taken));
	return entry1;
}
const char * TextEntry2() {
	static char entry2[] = "00";
	strcpy(entry2, GenerateText(entries[2].taken));
	return entry2;
}

#if ALLOW_SHOP
const char * TextEntry3() {
	static char entry3[] = "00";
	strcpy(entry3, GenerateText(entries[3].taken));
	return entry3;
}
#endif
	
const char * TextEntriesSize() {
	static char entrySizeText[] = "00";
	strcpy(entrySizeText, GenerateText(entriesSize));
	return entrySizeText;
}

void DebugMenuAppear(Window *window)
{
	MenuAppear(window);
	uint8_t i=0;
	ShowMainWindowRow(i++, "Debug", "");	
	ShowMainWindowRow(i++, entries[0].name, TextEntry0());
	ShowMainWindowRow(i++, entries[1].name, TextEntry1());
	ShowMainWindowRow(i++, entries[2].name, TextEntry2());
#if ALLOW_SHOP
	ShowMainWindowRow(i++, entries[3].name, TextEntry3());
#endif
	ShowMainWindowRow(i++, "EntriesSize", TextEntriesSize());
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

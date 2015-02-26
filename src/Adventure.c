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
#if ALLOW_RANDOM_DUNGEON_GRAPHICS	
static uint8_t lastImage = 0;
#endif
static bool adventureWindowVisible = false;

void AdventureWindowAppear(Window *window);
void AdventureWindowDisappear(Window *window);

static MenuDefinition adventureMenuDef = 
{
	.menuEntries = 
	{
		{"Go", "Use stamina and go", GoUsingStamina},
		{"Menu", "Open the main menu", ShowMainMenu}
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

static Window *adventureWindow = NULL;

void LoadRandomDungeonImage(void)
{
#if ALLOW_RANDOM_DUNGEON_GRAPHICS		
	uint8_t result = Random(12) + 1;
	if(result == lastImage) {
		result = (result + 3)  % 12;
	}
	if(result < 6)
		adventureMenuDef.mainImageId = RESOURCE_ID_IMAGE_DUNGEONSTRAIGHT;
	else if(result < 9)
		adventureMenuDef.mainImageId = RESOURCE_ID_IMAGE_DUNGEONLEFT;
	else if(result < 12)
		adventureMenuDef.mainImageId = RESOURCE_ID_IMAGE_DUNGEONRIGHT;
	else
		adventureMenuDef.mainImageId = RESOURCE_ID_IMAGE_DUNGEONDEADEND;
		
	lastImage = result;
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
	{ShowItemGainWindow, ITEM_CARDS,0, false},
	{ShowBattleWindow, BATTLE_CARDS,0,false},
	{ShowNewFloorWindow, FLOOR_CARDS,0,false},
	{ShowShopWindow, SHOP_CARDS,0,false}
};

#else
static Card entries[] = 
{
	{ShowItemGainWindow, ITEM_CARDS,0,false},
	{ShowBattleWindow, BATTLE_CARDS,0,false},
	{ShowNewFloorWindow, FLOOR_CARDS,0,false},
};
#endif
static uint8_t entriesSize =  NB_TYPE_CARDS;

#if EVENT_CHANCE_SCALING
static uint8_t ticksSinceLastEvent = 0;
#endif
uint8_t GetEntriesSize(void) {
	return entriesSize;
}
void SetEntriesSize(uint8_t entries) {
	entriesSize = entries;
}

void GetCardSaves(CardSave* saves) {
	memset(saves, 0, NB_TYPE_CARDS * sizeof(CardSave));
	for(uint8_t i = 0; i < NB_TYPE_CARDS; i++) {
		saves[i].taken = entries[i].taken;
		saves[i].empty = entries[i].empty;
		DEBUG_VERBOSE_LOG("Card Taken: %d\nisEmpty: %d", saves[i].taken, saves[i].empty);
	}
}

void SetCardSave(CardSave * saves) {
	for(uint8_t i = 0; i < NB_TYPE_CARDS; i++) {
		DEBUG_VERBOSE_LOG("Card Taken: %d\nisEmpty: %d", saves[i].taken, saves[i].empty);
		(&entries[i])->taken = saves[i].taken;
		(&entries[i])->empty = saves[i].empty;
	}
}


void ResetCurrentTaken() {
	INFO_LOG("Reset Deck of Adventure Cards");
	entriesSize =  NB_TYPE_CARDS;
	for(uint8_t i = 0; i < entriesSize; ++i) {
		(&entries[i])->taken = 0;
		(&entries[i])->empty = false;
	}
}

void ComputeAdventure() {
	
	uint16_t rand = 0;
	if(entriesSize != 1) {
		rand = Random(entriesSize);
	}	
	DEBUG_VERBOSE_LOG("Random: %d", rand);
	Card* card;
	do {
		 card = &entries[rand];
		 rand = (rand + 1) % NB_TYPE_CARDS;
	} while(card->empty);
	
	DEBUG_VERBOSE_LOG("Random Modified: %d", rand);	
	
	card->windowFunction();	
	card->taken += 1;	
	
	DEBUG_LOG("Card Taken: %d/%d", card->taken, card->total);	
	DEBUG_VERBOSE_LOG("Card Type Left: %d", entriesSize);
	
	if(card->taken == card->total) {
		entriesSize--;
		card->empty = true;
		if(entriesSize == 0) {
			ResetCurrentTaken();
		}
	}	
}

bool ComputeRandomEvent(bool fastMode)
{
	uint16_t chanceOfEvent = EVENT_CHANCE_BASE;
#if EVENT_CHANCE_SCALING
	if(ticksSinceLastEvent > 3) {
		chanceOfEvent += (ticksSinceLastEvent - 2) * 4;
	}
#endif
	if(!fastMode) {
		uint16_t result = Random(100) + 1;
		if(result > chanceOfEvent) {
			if(Random(3)) {
				HealStamina(1);
			}
			return false;
		}
	}
	
	if(GetVibration())
		vibes_short_pulse();
		
	ComputeAdventure();
	
#if EVENT_CHANCE_SCALING
		ticksSinceLastEvent = 0;
#endif
	return true;
}

void GoUsingStamina(void) { 
	if (SpendStamina()) {
		if(!UpdateAdventure()) {
			const char * stam = UpdateStaminaText();
			const uint8_t size = 12 * sizeof(char);
			char *avail = malloc(size);
			snprintf(avail, size, "%s%s", "Stamina: ", stam);
			SetMenuDescription(avail);
			free(avail);
		}
	} else {
		SetMenuDescription("Stamina depleted.");
	}
}

bool UpdateAdventure(void)
{
	if(!adventureWindowVisible)
		return false;
	
	if(IsBattleForced())
	{
		INFO_LOG("Triggering forced battle.");
		ShowBattleWindow();
		return true;
	}

#if EVENT_CHANCE_SCALING
	++ticksSinceLastEvent;
#endif
	if(updateDelay && !GetFastMode())
	{
		--updateDelay;
		return false;
	}

	bool result = ComputeRandomEvent(GetFastMode());
	LoadRandomDungeonImage();
	return result;
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

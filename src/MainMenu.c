#include "pebble.h"

#include "Adventure.h"
#include "Character.h"
#include "Battle.h"
#include "Items.h"
#include "MainMenu.h"
#include "Menu.h"
#include "Shop.h"
#include "UILayers.h"

// **************** TEST MENU ******************//

void DoNothing(void)
{
}

#if ALLOW_TEST_MENU

void ForceEvent(void)
{
	PopMenu();
	ComputeRandomEvent(true);
}

void ForceNewFloor(void)
{
	PopMenu();
	ShowNewFloorWindow();
}

void ForceItemGain(void)
{
	PopMenu();
	ShowItemGainWindow();
}

void ForceBattle(void)
{
	PopMenu();
	ShowBattleWindow();
}

void ForceShop(void)
{
	PopMenu();
	ShowShopWindow();
}

static MenuDefinition testMenuDef = 
{
	.menuEntries = 
	{
		{"Quit", "", PopMenu},
		{"Random", "", ForceEvent},
		{"NewFloor", "", ForceNewFloor},
		{"Item", "", ForceItemGain},
		{"Battle", "", ForceBattle},
		{"Shop", "", ForceShop},
	},
	.mainImageId = -1
};

void ShowTestMenu(void)
{
	PushNewMenu(&testMenuDef);
}
#endif

//************* Main Menu *****************//

static bool vibration = true;
static bool fastMode = false;
static bool easyMode = false;
#if ALLOW_GOD_MODE
static bool godMode = false;
#endif

void DrawOptionsMenu(void)
{
	ShowMainWindowRow(0, "Options", "");
	ShowMainWindowRow(1, "Vibration", vibration ? "On" : "Off");
	ShowMainWindowRow(2, "Fast Mode", fastMode ? "On" : "Off");
	ShowMainWindowRow(3, "Easy Mode", easyMode ? "On" : "Off");
#if ALLOW_GOD_MODE
	ShowMainWindowRow(4, "God Mode", godMode ? "On" : "Off");
#endif
}

void ToggleVibration(void)
{
	vibration = !vibration;
	DrawOptionsMenu();
}

bool GetVibration(void)
{
	return vibration;
}

bool GetFastMode(void)
{
	return fastMode;
}

void ToggleFastMode(void)
{
	fastMode = !fastMode;
	DrawOptionsMenu();
}

bool GetEasyMode(void)
{
	return easyMode;
}

void ToggleEasyMode(void)
{
	easyMode = !easyMode;
	DrawOptionsMenu();
}

#if ALLOW_GOD_MODE
void ToggleGodMode(void)
{
	godMode = !godMode;
	DrawOptionsMenu();
}

bool GetGodMode(void)
{
	return godMode;
}

void GodModAdd(MenuEntry *menuEntries) {	
	MenuEntry newEntry =  {"Toggle", "Invinsible and autokill", ToggleGodMode};
	menuEntries[4] = newEntry;
}
#endif
void OptionsMenuAppear(Window *window);

static MenuDefinition optionsMenuDef = 
{
	.menuEntries = 
	{
		{"Quit", "Return to main menu", PopMenu},
		{"Toggle", "Toggle Vibration", ToggleVibration},
		{"Toggle", "Speed up events", ToggleFastMode},
		{"Toggle", "Asked to grab the ladder", ToggleEasyMode}
	},
	.appear = OptionsMenuAppear,
#if ALLOW_GOD_MODE
	.modify = GodModAdd,
#endif
	.mainImageId = -1
	
};

void OptionsMenuAppear(Window *window)
{
	MenuAppear(window);
	DrawOptionsMenu();
}

void ShowOptionsMenu(void)
{
	PushNewMenu(&optionsMenuDef);
}

void MainMenuWindowAppear(Window *window);

static MenuDefinition mainMenuDef = 
{
	.menuEntries = 
	{
		{"Quit", "Return to adventure", PopMenu},
		{"Items", "Items Owned", ShowMainItemMenu},
		{"Progress", "Character advancement", ShowProgressMenu},
		{"Stats", "Character Stats", ShowStatMenu},
		{"Options", "Open the options menu", ShowOptionsMenu},
	},
	.appear = MainMenuWindowAppear,
	.mainImageId = RESOURCE_ID_IMAGE_REST
};

void MainMenuWindowAppear(Window *window)
{
	MenuAppear(window);
	ShowMainWindowRow(0, "Paused", "");
}

void ShowMainMenu(void)
{
	PushNewMenu(&mainMenuDef);
}

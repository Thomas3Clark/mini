#pragma once

#include "MiniDungeon.h"
typedef void (*MenuFunction)(void);

typedef struct MenuEntry
{
	const char *text;
	const char *description;
	MenuFunction menuFunction;
} MenuEntry;

typedef void (*DynamicMenuEntries)(struct MenuEntry *menuEntries);

typedef struct
{
	MenuEntry menuEntries[MAX_MENU_ENTRIES];
	DynamicMenuEntries modify;
	WindowHandler init;
	WindowHandler deinit;
	WindowHandler appear;
	WindowHandler disappear;
	int currentSelection : 4;
	int animated : 1;
	int disableBackButton : 1;
	int mainImageId : 6;
} MenuDefinition;

void SetCurrentMenu(MenuDefinition *menuDef);

void SetMenuClickConfigProvider(Window *window);

// Useful callback while building a new menu
void DoNothing(void);

void MenuInit(Window *window);
void MenuDeinit(Window *window);
void MenuAppear(Window *window);
void MenuDisappear(Window *window);
void PushNewMenu(MenuDefinition *menuDef);
void PopMenu(void);

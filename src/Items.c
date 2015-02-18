#include "pebble.h"

#include "Character.h"
#include "Items.h"
#include "Menu.h"
#include "Adventure.h"
#include "UILayers.h"
#include "Utils.h"

static ItemData itemData[] = 
{
	{"Potion", "00",50,10,0},
	{"Elixir", "00",5,100,0},
	{"Bomb", "00",15,20,0},
	{"Icicle", "00",15,20,0},
	{"Spark", "00",15,20,0}
};

const char *UpdateItemCountText(ItemType itemType)
{
#if BOUNDS_CHECKING
	if(itemType >= ITEM_TYPE_COUNT)
		return "";
#endif
	ItemData* item = GetItem(itemType);
	UIntToString(item->countText, item->owned);
	return item->countText;
}

void ClearInventory(void)
{
	int i;
	for(i = 0; i < ITEM_TYPE_COUNT; ++i)
	{
		GetItem(i)->owned = 0;
	}
}

ItemData *GetItem(ItemType itemType) {
	return &itemData[itemType];
}

const char *GetItemName(ItemType itemType)
{
#if BOUNDS_CHECKING
	if(itemType >= ITEM_TYPE_COUNT)
		return "";
#endif

	return GetItem(itemType)->name;
}

void ItemGainMenuInit(Window *window);
void ItemGainMenuAppear(Window *window);

static MenuDefinition itemGainMenuDef = 
{
	.menuEntries = 
	{
		{"Ok", "Return to adventuring", PopMenu}
	},
	.init = ItemGainMenuInit,
	.appear = ItemGainMenuAppear,
	.mainImageId = -1
};

void ShowAllItemCounts(void)
{
	int i;
	ShowMainWindowRow(0, "Items", "");
	for(i = 0; i < ITEM_TYPE_COUNT; ++i)
	{
		ShowMainWindowRow(i + 1, GetItemName(i), UpdateItemCountText(i));
	}
}

ItemType typeGained;

void ItemGainMenuAppear(Window *window)
{
	MenuAppear(window);
	ShowMainWindowRow(0, "Item Gained", "");
	ShowMainWindowRow(1, GetItemName(typeGained), UpdateItemCountText(typeGained));
}

bool AddItem(ItemType type)
{
	ItemData* item = GetItem(type);

	if(item->owned + 1 > 99)
	{
		return false;
	}
	item->owned++;
	return true;
}
bool RemoveItem(ItemType type) {
	ItemData* item = GetItem(type);
	
	if(item->owned <= 0) {
		return false;
	}
	item->owned--;
	return true;
}
void ItemGainMenuInit(Window *window)
{
	int result = Random(100) + 1;
	int i = 0;
	int acc = 0;
	MenuInit(window);
	do
	{
		acc += itemData[i].probability;
		if(acc >= result)
		{
			typeGained = i;
			AddItem(i);
			break;
		}
		++i;      
	} while (i < ITEM_TYPE_COUNT);
}

void ShowItemGainWindow(void)
{
	PushNewMenu(&itemGainMenuDef);
}

bool AttemptToUseHealingItem(ItemType type, int power)
{
	ItemData* item = GetItem(type);
	if(item->owned > 0 && PlayerIsInjured())
	{
		HealPlayerByPercent(power);
		--item->owned;
		ShowAllItemCounts();
		return true;
	}  
	return false;
}

bool AttemptToUsePotion(void)
{
	return AttemptToUseHealingItem(ITEM_TYPE_POTION, 50);
}

void ActivatePotion(void)
{
	AttemptToUsePotion();
}

bool AttemptToUseFullPotion(void)
{
	return AttemptToUseHealingItem(ITEM_TYPE_FULL_POTION, 100);
}

void ActivateFullPotion(void)
{
	AttemptToUseFullPotion();
}

void ItemMainMenuAppear(Window *window);

static MenuDefinition itemMainMenuDef = 
{
	.menuEntries = 
	{
		{"Quit", "Return to main menu", PopMenu},
		{"Drink", "Heal 50% of max health", ActivatePotion},
		{"Drink", "Heal 100% of max health", ActivateFullPotion}
	},
	.appear = ItemMainMenuAppear,
	.mainImageId = -1
};

void ItemMainMenuAppear(Window *window)
{
	MenuAppear(window);
	ShowAllItemCounts();
}

void ShowMainItemMenu(void)
{
	PushNewMenu(&itemMainMenuDef);
}

bool AttemptToUseItem(ItemType type)
{
	ItemData* item = GetItem(type);
	if(item->owned > 0)
	{
		--item->owned;
		return true;
	}
	
	return false;
}

bool AttemptToConsumeFireScroll(void)
{
	return AttemptToUseItem(ITEM_TYPE_FIRE_SCROLL);
}

bool AttemptToConsumeIceScroll(void)
{
	return AttemptToUseItem(ITEM_TYPE_ICE_SCROLL);
}

bool AttemptToConsumeLightningScroll(void)
{
	return AttemptToUseItem(ITEM_TYPE_LIGHTNING_SCROLL);
}

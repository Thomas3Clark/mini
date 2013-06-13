#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "Character.h"
#include "Items.h"
#include "Menu.h"
#include "Adventure.h"
#include "UILayers.h"
#include "Utils.h"

typedef struct
{
	const char *name;
	char countText[2];
	uint8_t randomChance;
	uint8_t owned;
} ItemData;

ItemData itemData[ITEM_TYPE_COUNT] = 
{
	{"Potion", "00",50,0},
	{"Elixir", "00",5,0},
	{"Bomb", "00",15,0},
	{"Icicle", "00",15,0},
	{"Spark", "00",15,0}
};

const char *UpdateItemCountText(ItemType itemType)
{
#if BOUNDS_CHECKING
	if(itemType >= ITEM_TYPE_COUNT)
		return "";
#endif
		
	IntToString(itemData[itemType].countText, 2, itemData[itemType].owned);
	return itemData[itemType].countText;
}

void ClearInventory(void)
{
	int i;
	for(i = 0; i < ITEM_TYPE_COUNT; ++i)
	{
		itemData[i].owned = 0;
	}
}

const char *GetItemName(ItemType itemType)
{
#if BOUNDS_CHECKING
	if(itemType >= ITEM_TYPE_COUNT)
		return "";
#endif

	return itemData[itemType].name;
}

void ItemGainMenuInit(Window *window);
void ItemGainMenuAppear(Window *window);

MenuDefinition itemGainMenuDef = 
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
	uint8_t i;
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
	itemData[type].owned++;
	if(itemData[type].owned > 99)
	{
		itemData[type].owned = 99;
		return false;
	}
	return true;
}

void ItemGainMenuInit(Window *window)
{
	int result = Random(100);
	int i = 0;
	int acc = 0;
	MenuInit(window);
	do
	{
		acc += itemData[i].randomChance;
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

bool AttemptToUseHealingItem(ItemType type, uint8_t power)
{
	if(itemData[type].owned > 0 && PlayerIsInjured())
	{
		HealPlayerByPercent(power);
		--itemData[type].owned;
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

MenuDefinition itemMainMenuDef = 
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
	if(itemData[type].owned > 0)
	{
		--itemData[type].owned;
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

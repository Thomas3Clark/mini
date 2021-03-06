#include "pebble.h"

#include "Character.h"
#include "Items.h"
#include "Logging.h"
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

void UpdateItemCountText(ItemData* item)
{
	UIntToString(item->countText, item->owned);
}

void GetItemsOwned(uint8_t * itemsOwned) {
	for(uint8_t i = 0; i < ITEM_TYPE_COUNT; i++) {
		DEBUG_LOG("Get Item %s: %u", itemData[i].name, itemData[i].owned);
		itemsOwned[i] = itemData[i].owned;
	}
}

void SetItemOwned(uint8_t* itemsOwned) {
	for(uint8_t i = 0; i < ITEM_TYPE_COUNT; i++) {
		itemData[i].owned = itemsOwned[i];
		UpdateItemCountText(&itemData[i]);
		DEBUG_LOG("Set Item %s: %u", itemData[i].name, itemData[i].owned);
	}
}

void ClearInventory(void)
{
	int i;
	GetItem(0)->owned = 3;
	for(i = 1; i < ITEM_TYPE_COUNT; ++i)
	{
		GetItem(i)->owned = 0;
	}
}

ItemData *GetItem(ItemType itemType) {
	#if BOUNDS_CHECKING
	if(itemType >= ITEM_TYPE_COUNT)
		ERROR_LOG("Item Out of Bound: %d", itemType);
		return NULL;
	#endif
	return &itemData[itemType];
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
		UpdateItemCountText(&itemData[i]);
		ShowMainWindowRow(i + 1, itemData[i].name, itemData[i].countText);
	}
}

ItemData* itemGained;

void ItemGainMenuAppear(Window *window)
{
	MenuAppear(window);
	ShowMainWindowRow(0, "Item Gained", "");
	UpdateItemCountText(itemGained);
	ShowMainWindowRow(1, itemGained->name, itemGained->countText);
}

bool AddItem(ItemData* item)
{
	if(item->owned + 1 > 99)
	{
		return false;
	}
	item->owned++;
	return true;
}
bool RemoveItem(ItemData* item) {

	if(item->owned <= 0) {
		return false;
	}
	item->owned--;
	return true;
}
void ItemGainMenuInit(Window *window)
{
	itemGained = NULL;
	int result = Random(100) + 1;
	int i = 0;
	int acc = 0;
	MenuInit(window);
	do
	{
		acc += itemData[i].probability;
		if(acc >= result)
		{
			itemGained = &itemData[i];
			DEBUG_LOG("Got Item %s: %u", itemGained->name, itemGained->owned);
			AddItem(itemGained);
			DEBUG_LOG("After AddItem %s: %u", itemGained->name, itemGained->owned);
			break;
		}
		++i;
	} while (i < ITEM_TYPE_COUNT);
}

void ShowItemGainWindow(void)
{
	INFO_LOG("Item gained.");
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

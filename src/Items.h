#pragma once

typedef enum
{
	ITEM_TYPE_POTION = 0,
	ITEM_TYPE_FULL_POTION,
	ITEM_TYPE_FIRE_SCROLL,
	ITEM_TYPE_ICE_SCROLL,
	ITEM_TYPE_LIGHTNING_SCROLL,
	ITEM_TYPE_COUNT
} ItemType;

typedef struct ItemData
{
	const char *name;
	char countText[2];
	uint8_t probability;
	uint8_t cost;
	uint8_t owned;
} ItemData;

void ShowItemGainWindow(void);
void ShowMainItemMenu(void);

bool AttemptToUsePotion(void);
bool AttemptToUseFullPotion(void);
bool AttemptToConsumeFireScroll(void);
bool AttemptToConsumeIceScroll(void);
bool AttemptToConsumeLightningScroll(void);
bool AddItem(ItemType type);
bool RemoveItem(ItemType type);

void ShowAllItemCounts(void);
void ClearInventory(void);

ItemData *GetItem(ItemType itemType);

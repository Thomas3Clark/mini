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

typedef struct
{
	const char *name;
	char countText[2];
	uint8_t randomChance;
	uint8_t owned;
	uint8_t price;
} ItemData;

void ShowItemGainWindow(void);
void ShowMainItemMenu(void);

bool AttemptToUsePotion(void);
bool AttemptToUseFullPotion(void);
bool AttemptToConsumeFireScroll(void);
bool AttemptToConsumeIceScroll(void);
bool AttemptToConsumeLightningScroll(void);
bool AddItem(ItemType type);

void ShowAllItemCounts(void);
void ClearInventory(void);

ItemData* GetItemData(ItemType type);

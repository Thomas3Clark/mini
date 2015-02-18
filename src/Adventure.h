#pragma once

typedef void (*ShowWindowFunction)(void);
typedef enum {
	CARD_ITEM,
	CARD_BATTLE,
	CARD_FLOOR,
	CARD_SHOP
} CardType;

typedef struct {
	uint8_t taken;
	uint8_t total;
	CardType type;
} CardMetadata;

typedef struct
{
	ShowWindowFunction windowFunction;
	const char *name;
	CardMetadata meta;	
} Card;

const char *UpdateFloorText(void);

void ToggleVibration(void);
const char *UpdateVibrationText(void);

void ShowAdventureWindow(void);
void ShowNewFloorWindow(void);

bool ComputeRandomEvent(bool fastMode);
void UpdateAdventure(void);

void ResetCurrentTaken(void);

CardMetadata * GetCardsMeta(void);
void LoadCardsMeta(CardMetadata* data);

uint8_t GetEntriesSize(void);
void SetEntriesSize(uint8_t size);

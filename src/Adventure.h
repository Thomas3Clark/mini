#pragma once

typedef void (*ShowWindowFunction)(void);

typedef struct
{
	ShowWindowFunction windowFunction;
	uint8_t number;
	uint8_t current;
} CardDeck;

const char *UpdateFloorText(void);

void ToggleVibration(void);
const char *UpdateVibrationText(void);

void ShowAdventureWindow(void);
void ShowNewFloorWindow(void);

bool ComputeRandomEvent(bool fastMode);
void UpdateAdventure(void);

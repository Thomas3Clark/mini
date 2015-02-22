#pragma once

typedef void (*ShowWindowFunction)(void);

typedef struct
{
	ShowWindowFunction windowFunction;
	uint8_t total;
	uint8_t taken;
	bool empty;
} Card;

const char *UpdateFloorText(void);

void ToggleVibration(void);
const char *UpdateVibrationText(void);

void ShowAdventureWindow(void);
void ShowNewFloorWindow(void);

bool ComputeRandomEvent(bool fastMode);
void UpdateAdventure(void);

void ResetCurrentTaken(void);

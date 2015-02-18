#pragma once
#include "MonsterInfo.h"

void ShowBattleWindow(void);

bool ClosingWhileInBattle(void);

void IncrementFloor(void);
uint8_t GetCurrentFloor(void);
void SetCurrentFloor(int);
void ResetFloor(void);

void ResumeBattle();
bool IsBattleForced(void);

#pragma once

void ShowBattleWindow(void);

bool ClosingWhileInBattle(void);
int GetCurrentMonsterHealth(void);

void IncrementFloor(void);
uint8_t GetCurrentFloor(void);
void SetCurrentFloor(int);
void ResetFloor(void);

void ResumeBattle(CurrentMonster currentMonster, int currentMonsterHealth);
bool IsBattleForced(void);

#pragma once

void ShowMainMenu(void);
bool GetVibration(void);
bool GetFastMode(void);
bool GetEasyMode(void);
void ShowTestMenu(void);

#if ALLOW_GOD_MODE
bool GetGodMode(void);
#endif

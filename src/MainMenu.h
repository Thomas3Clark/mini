#pragma once

void ShowMainMenu(void);
bool GetVibration(void);
void SetVibration(bool enable);
bool GetFastMode(void);
void SetFastMode(bool enable);
void ShowTestMenu(void);
bool GetEasyMode(void);

#if ALLOW_GOD_MODE
bool GetGodMode(void);
#endif

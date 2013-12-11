#pragma once

#define MAX_MAIN_TEXT_LAYERS 6

void SetMenuHighlight(int menuItem, bool selected);
void SetMenuDescription(const char *desc);
void ShowMenuLayer(int index, const char *text);
void HideMenuLayer(int index);
void HideAllMenuLayers(void);

void LoadMainBmpImage(Window *window, int resourceId);
void UnloadMainBmpImage(void);
void UnloadBackgroundImage(void);
void UnloadTextLayers(void);

void ShowMainWindowRow(int index, const char *text, const char *number);

void UpdateClock(void);
void UpdateLevelLayerText(int level);

void UpdateHealthText(int16_t currentHealth, uint16_t maxHealth);

TextLayer * InitializeTextLayer(GRect frame, GFont font);

Window * InitializeConfirmationWindow(TextLayer *exitText, TextLayer *yesText, TextLayer *noText);
void InitializeExitConfirmationWindow(void);
Window * InitializeWindow(const char *name);
Window * InitializeMenuWindow(const char *name, WindowHandler init, WindowHandler deinit, WindowHandler appear, WindowHandler disappear);

void WindowAppear(Window *window);
void WindowDisappear(Window *window);

#pragma once

// Game tuning 
#define STAT_POINTS_PER_LEVEL 2
#define XP_FOR_NEXT_LEVEL 5 * level
#define EVENT_CHANCE_BASE 35
#define SHUFFLE_DECK_TIMES 10
#define SALE_PERCENT 0.65 //Need to change the price in sell window

//Card tuning
#define SHOP_CARDS 2
#define BATTLE_CARDS 8
#define FLOOR_CARDS 2
#define ITEM_CARDS 10

// Feature tuning
#define PAD_WITH_SPACES 1

// Publish 0 turns on the test menu and ignores the official setting 
// The back button override is always on in test mode
// God mode activated in test mode
#define PUBLISH 0
// Official 0 allows the back button override, while official 1 does not
#define OFFICIAL 0

// Features to turn off to make space
#define ALLOW_RANDOM_DUNGEON_GRAPHICS 1
#define BOUNDS_CHECKING 1
#define EVENT_CHANCE_SCALING 1

#define ALLOW_SHOP 1
#if ALLOW_SHOP
	#define ALLOW_ITEM_SHOP 1
	#define ALLOW_STAT_SHOP 1
	#define NB_CARDS (SHOP_CARDS + BATTLE_CARDS + ITEM_CARDS + FLOOR_CARDS)
#else
	#define ALLOW_ITEM_SHOP 0
	#define ALLOW_STAT_SHOP 0
	#define NB_CARDS (BATTLE_CARDS + ITEM_CARDS + FLOOR_CARDS)
#endif	

// Set up the back button and test menu based on previous choices
#if PUBLISH
	#if OFFICIAL
		#define OVERRIDE_BACK_BUTTON 0
	#else
		#define OVERRIDE_BACK_BUTTON 1
	#endif
	#define ALLOW_TEST_MENU 0
	#define	ALLOW_GOD_MODE	0
#else
	// Overriding the back button is not officially supported, but works
	#define OVERRIDE_BACK_BUTTON 1
	#define ALLOW_TEST_MENU 1
	#define	ALLOW_GOD_MODE	1
#endif

// This needs to be large enough to handle the maximum size of the window stack
#define MAX_MENU_WINDOWS 4
// This determines how many text rows there are in the interface
#define MAX_MENU_ENTRIES 6

void ResetGame(void);

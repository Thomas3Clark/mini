#include "pebble.h"

#include "Battle.h"
#include "Character.h"
#include "Items.h"
#include "Logging.h"
#include "MainMenu.h"
#include "Monsters.h"
#include "Persistence.h"
#include "Shop.h"
#include "MonsterInfo.h"
	
#define CURRENT_DATA_VERSION 2
enum
{
	PERSISTED_IS_DATA_SAVED = 0,
	PERSISTED_CURRENT_DATA_VERSION,
	PERSISTED_MAX_KEY_USED,
	PERSISTED_CHARACTER_DATA,
	PERSISTED_CURRENT_FLOOR,
	PERSISTED_ITEM_DATA,
	PERSISTED_STAT_POINTS_PURCHASED,
	PERSISTED_VIBRATION,
	PERSISTED_FAST_MODE,
	PERSISTED_EASY_MODE,
	
	PERSISTED_IN_COMBAT,
	PERSISTED_MONSTER_TYPE,
	PERSISTED_MONSTER_HEALTH,
	
	// This needs to always be last
	PERSISTED_DATA_COUNT
};

#define MAX_PERSISTED_KEY PERSISTED_DATA_COUNT - 1

bool IsPersistedDataCurrent(void)
{
	bool dataSaved = persist_read_bool(PERSISTED_IS_DATA_SAVED);
	int savedVersion;
	if(!dataSaved)
		return true;
	
	savedVersion = persist_read_int(PERSISTED_CURRENT_DATA_VERSION);
	
	return savedVersion == CURRENT_DATA_VERSION;
}
	
void ClearPersistedData(void)
{
	if(persist_exists(PERSISTED_IS_DATA_SAVED))
	{
		DEBUG_LOG("Clearing persisted data.");
		int maxKey = persist_read_int(PERSISTED_MAX_KEY_USED);
		int i;
		for(i = 0; i <= maxKey; ++i)
		{
			persist_delete(i);
		}
	}
}
	
bool SavePersistedData(void)
{
	CharacterData *characterData;

	if(!IsPersistedDataCurrent())
	{
		WARNING_LOG("Persisted data does not match current version, clearing.");
		ClearPersistedData();
	}
	
	if(sizeof(CharacterData) > PERSIST_DATA_MAX_LENGTH )
	{
		ERROR_LOG("CharacterData is too big to save (%d).", sizeof(CharacterData));
		return false;
	}

	INFO_LOG("Saving persisted data.");
	persist_write_bool(PERSISTED_IS_DATA_SAVED, true);
	persist_write_int(PERSISTED_CURRENT_DATA_VERSION, CURRENT_DATA_VERSION);
	persist_write_int(PERSISTED_MAX_KEY_USED, MAX_PERSISTED_KEY);
	
	characterData = GetCharacter();
	persist_write_data(PERSISTED_CHARACTER_DATA, characterData, sizeof(CharacterData));
	
	persist_write_int(PERSISTED_CURRENT_FLOOR, GetCurrentFloor());
	
	uint8_t *itemsOwned = GetItemsOwned();
	persist_write_data(PERSISTED_ITEM_DATA, itemsOwned, sizeof(itemsOwned));
	free(itemsOwned);
	
	persist_write_int(PERSISTED_STAT_POINTS_PURCHASED, GetStatPointsPurchased());

	persist_write_bool(PERSISTED_VIBRATION, GetVibration());
	persist_write_bool(PERSISTED_FAST_MODE, GetFastMode());
	persist_write_bool(PERSISTED_EASY_MODE, GetEasyMode());

	persist_write_bool(PERSISTED_IN_COMBAT, ClosingWhileInBattle());
	
	persist_write_data(PERSISTED_MONSTER_TYPE, GetCurMonster(), sizeof(MonsterInfo));
	
	return true;
}

bool LoadPersistedData(void)
{
	CharacterData *characterData;
	int floor = 0;
	if(!persist_exists(PERSISTED_IS_DATA_SAVED) || !persist_read_bool(PERSISTED_IS_DATA_SAVED))
		return false;
		
	if(!IsPersistedDataCurrent())
	{
		WARNING_LOG("Persisted data does not match current version, clearing.");
		ClearPersistedData();
		return false;
	}

	INFO_LOG("Loading persisted data.");
	characterData = GetCharacter();
	persist_read_data(PERSISTED_CHARACTER_DATA, characterData, sizeof(CharacterData));
	floor = persist_read_int(PERSISTED_CURRENT_FLOOR);
	SetCurrentFloor(floor);
	
	uint8_t *itemsOwned = GetItemsOwned();
	persist_read_data(PERSISTED_ITEM_DATA, itemsOwned, sizeof(itemsOwned));
	SetItemOwned(itemsOwned);
	free(itemsOwned);
	
	SetStatPointsPurchased(persist_read_int(PERSISTED_STAT_POINTS_PURCHASED));
	SetVibration(persist_read_bool(PERSISTED_VIBRATION));
	SetFastMode(persist_read_bool(PERSISTED_FAST_MODE));
	SetEasyMode(persist_read_bool(PERSISTED_EASY_MODE));
	
	if(persist_read_bool(PERSISTED_IN_COMBAT))
	{
		persist_read_data(PERSISTED_MONSTER_TYPE, GetCurMonster(), sizeof(MonsterInfo));
		ResumeBattle();
	}
	if(characterData->level == 0)
	{
		// Something bad happened to the data, possible due to a watch crash
		ERROR_LOG("Persisted data was broken somehow, clearing");
		ClearPersistedData();
		return false;
	}
	return true;
}

#pragma once

// TODO: turn the list of stats into an array for more code reuse
typedef struct
{
	uint16_t maxHealth;
	uint16_t currentHealth;
	uint8_t strength;
	uint8_t magic;
	uint8_t defense;
	uint8_t magicDefense;
} Stats;

typedef struct 
{
	uint16_t xp;
	uint16_t xpForNextLevel;
	uint16_t level;
	uint16_t gold;
	uint16_t escapes;
	Stats stats;
} CharacterData;

uint16_t ComputePlayerHealth(uint16_t level);
void UpdateCharacterHealth(void);
void UpdateCharacterLevel(void);
const char *UpdateGoldText(void);
void AddStatPointToSpend(void);
void IncrementEscapes(void);

void InitializeCharacter(void);

bool DealPlayerDamage(int damage);
CharacterData *GetCharacter(void);

void HealPlayerByPercent(int percent);
bool PlayerIsInjured(void);

bool GrantExperience(int exp);
void GrantGold(int gold);

void ShowStatMenu(void);
void ShowProgressMenu(void);

void LevelUp(void);
void ShowEndWindow(void);

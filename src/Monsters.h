#pragma once

#include "Character.h"

typedef struct
{
	const char *name;
	uint16_t imageId : 6;
	uint16_t extraFireDefenseMultiplier : 11;
	uint16_t extraIceDefenseMultiplier : 11;
	uint16_t extraLightningDefenseMultiplier : 11;
	bool allowMagicAttack : 1;
	bool allowPhysicalAttack : 1;
	uint16_t powerLevel : 3;
	uint16_t healthLevel : 3;
	uint16_t defenseLevel : 3;
	uint16_t magicDefenseLevel : 3;
	uint16_t goldScale : 4;
} MonsterDef;

typedef struct 
{
	uint8_t nbMonster;
	MonsterDef *monsters[];
} GroupMonsters;

int GetMonsterDefense(int defenseLevel);
int ScaleMonsterHealth(MonsterDef *monster, int baseHealth);
int GetMonsterPowerDivisor(int powerLevel);

MonsterDef *GetRandomMonster(int floor);

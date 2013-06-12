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
	uint8_t powerLevel : 3;
	uint8_t healthLevel : 3;
	uint8_t defenseLevel : 3;
	uint8_t magicDefenseLevel : 3;
	uint8_t goldScale : 4;
} MonsterDef;

typedef struct 
{
	uint8_t nbMonster;
	MonsterDef *monsters[];
} GroupMonsters;

uint8_t GetMonsterDefense(uint8_t defenseLevel);
uint16_t ScaleMonsterHealth(MonsterDef *monster, uint16_t baseHealth);
uint8_t GetMonsterPowerDivisor(uint8_t powerLevel);

MonsterDef *GetRandomMonster(uint8_t floor);

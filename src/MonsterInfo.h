#pragma once

typedef struct 
{
	uint8_t monsterGroup;
	uint8_t monsterId;
	int16_t health;
} MonsterInfo;

MonsterInfo* GetCurMonster();

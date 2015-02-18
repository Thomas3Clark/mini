#pragma once

typedef struct 
{
	uint8_t monsterGroup;
	uint8_t monsterId;
	uint16_t health;
} MonsterInfo;

static MonsterInfo currentMonsterInfo;

inline static MonsterInfo* GetCurMonster() {
	return &currentMonsterInfo;
}

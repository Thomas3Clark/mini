#include "pebble.h"

#include "MonsterInfo.h"


static MonsterInfo currentMonsterInfo;

MonsterInfo* GetCurMonster() {
	return &currentMonsterInfo;
}

void SetCurMonster(MonsterInfo *info) {
	currentMonsterInfo.monsterGroup = info->monsterGroup;
	currentMonsterInfo.monsterId = info->monsterId;
	currentMonsterInfo.health = info->health;
}


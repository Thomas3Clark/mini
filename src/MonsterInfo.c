#include "pebble.h"

#include "MonsterInfo.h"


static MonsterInfo currentMonsterInfo;

MonsterInfo* GetCurMonster() {
	return &currentMonsterInfo;
}

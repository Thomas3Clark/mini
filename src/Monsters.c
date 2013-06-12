#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "Monsters.h"
#include "Utils.h"


MonsterDef Rat = 
{
	.name = "Rat",
	.imageId = RESOURCE_ID_IMAGE_RAT,
	.extraFireDefenseMultiplier = 100,
	.extraIceDefenseMultiplier = 100,
	.extraLightningDefenseMultiplier = 100,
	.powerLevel = 0,
	.healthLevel = 0,
	.defenseLevel = 0,
	.magicDefenseLevel = 0,
	.allowPhysicalAttack = true,
	.goldScale = 0
};

MonsterDef MagPie = 
{
	.name = "Mag Pie",
	.imageId = RESOURCE_ID_IMAGE_MAGPIE,
	.extraFireDefenseMultiplier = 100,
	.extraIceDefenseMultiplier = 100,
	.extraLightningDefenseMultiplier = 100,
	.powerLevel = 0,
	.healthLevel = 0,
	.defenseLevel = 0,
	.magicDefenseLevel = 0,
	.allowPhysicalAttack = true,
	.goldScale = 1
};

MonsterDef Goblin = 
{
	.name = "Goblin",
	.imageId = RESOURCE_ID_IMAGE_GOBLIN,
	.extraFireDefenseMultiplier = 100,
	.extraIceDefenseMultiplier = 200,
	.extraLightningDefenseMultiplier = 100,
	.powerLevel = 1,
	.healthLevel = 1,
	.defenseLevel = 1,
	.magicDefenseLevel = 0,
	.allowPhysicalAttack = true,
	.goldScale = 2
};

MonsterDef Witch = 
{
	.name = "Witch",
	.imageId = RESOURCE_ID_IMAGE_WITCH,
	.extraFireDefenseMultiplier = 100,
	.extraIceDefenseMultiplier = 100,
	.extraLightningDefenseMultiplier = 100,
	.powerLevel = 1,
	.healthLevel = 1,
	.defenseLevel = 1,
	.magicDefenseLevel = 1,
	.allowMagicAttack = true,
	.goldScale = 2
};

MonsterDef Wizard = 
{
	.name = "Wizard",
	.imageId = RESOURCE_ID_IMAGE_WIZARD,
	.extraFireDefenseMultiplier = 100,
	.extraIceDefenseMultiplier = 100,
	.extraLightningDefenseMultiplier = 100,
	.powerLevel = 1,
	.healthLevel = 1,
	.defenseLevel = 1,
	.magicDefenseLevel = 3,
	.allowMagicAttack = true,
	.goldScale = 3
};

MonsterDef HalfZombie = 
{
	.name = "Half Zombie",
	.imageId = RESOURCE_ID_IMAGE_HALF_ZOMBIE,
	.extraFireDefenseMultiplier = 400,
	.extraIceDefenseMultiplier = 100,
	.extraLightningDefenseMultiplier = 100,
	.powerLevel = 1,
	.healthLevel = 1,
	.defenseLevel = 2,
	.magicDefenseLevel = 1,
	.allowPhysicalAttack = true,
	.goldScale = 1
};

MonsterDef Zombie = 
{
	.name = "Zombie",
	.imageId = RESOURCE_ID_IMAGE_ZOMBIE,
	.extraFireDefenseMultiplier = 300,
	.extraIceDefenseMultiplier = 100,
	.extraLightningDefenseMultiplier = 100,
	.powerLevel = 2,
	.healthLevel = 1,
	.defenseLevel = 1,
	.magicDefenseLevel = 1,
	.allowPhysicalAttack = true,
	.goldScale = 1
};

MonsterDef Bunny = 
{
	.name = "Bunny",
	.imageId = RESOURCE_ID_IMAGE_BUNNY,
	.extraFireDefenseMultiplier = 700,
	.extraIceDefenseMultiplier = 200,
	.extraLightningDefenseMultiplier = 100,
	.powerLevel = 1,
	.healthLevel = 1,
	.defenseLevel = 2,
	.magicDefenseLevel = 1,
	.allowPhysicalAttack = true,
	.goldScale = 0	
};

MonsterDef Turtle = 
{
	.name = "Turtle",
	.imageId = RESOURCE_ID_IMAGE_TURTLE,
	.extraFireDefenseMultiplier = 100,
	.extraIceDefenseMultiplier = 100,
	.extraLightningDefenseMultiplier = 600,
	.powerLevel = 1,
	.healthLevel = 2,
	.defenseLevel = 3,
	.magicDefenseLevel = 1,
	.allowPhysicalAttack = true,
	.goldScale = 0	
};

MonsterDef Cyclope = 
{
	.name = "Cyclope",
	.imageId = RESOURCE_ID_IMAGE_CYCLOPE,
	.extraFireDefenseMultiplier = 700,
	.extraIceDefenseMultiplier = 100,
	.extraLightningDefenseMultiplier = 200,
	.powerLevel = 2,
	.healthLevel = 1,
	.defenseLevel = 2,
	.magicDefenseLevel = 1,
	.allowMagicAttack = true,
	.goldScale = 2
};

MonsterDef Chimera = 
{
	.name = "Chimera",
	.imageId = RESOURCE_ID_IMAGE_CHIMERA,
	.extraFireDefenseMultiplier = 600,
	.extraIceDefenseMultiplier = 100,
	.extraLightningDefenseMultiplier = 100,
	.powerLevel = 2,
	.healthLevel = 3,
	.defenseLevel = 2,
	.magicDefenseLevel = 1,
	.allowMagicAttack = true,
	.allowPhysicalAttack = true,
	.goldScale = 2
};

MonsterDef Lich = 
{
	.name = "Lich",
	.imageId = RESOURCE_ID_IMAGE_LICH,
	.extraFireDefenseMultiplier = 100,
	.extraIceDefenseMultiplier = 600,
	.extraLightningDefenseMultiplier = 100,
	.powerLevel = 2,
	.healthLevel = 2,
	.defenseLevel = 3,
	.magicDefenseLevel = 1,
	.allowMagicAttack = true,
	.goldScale = 4
};

MonsterDef Dragon = 
{
	.name = "Dragon",
	.imageId = RESOURCE_ID_IMAGE_DRAGON,
	.extraFireDefenseMultiplier = 100,
	.extraIceDefenseMultiplier = 100,
	.extraLightningDefenseMultiplier = 100,
	.powerLevel = 2,
	.healthLevel = 2,
	.defenseLevel = 2,
	.magicDefenseLevel = 2,
	.allowPhysicalAttack = true,
	.allowMagicAttack = true,
	.goldScale = 10	
};

GroupMonsters FirstLevels = 
{
	.nbMonster = 2,
	.monsters = {&Rat,&MagPie}
};

GroupMonsters SecondLevels = 
{
	.nbMonster = 2,
	.monsters = {&Goblin,&Witch}
};

GroupMonsters ThirdLevels = 
{
	.nbMonster = 2,
	.monsters = {&Wizard,&HalfZombie}
	
};

GroupMonsters ForthLevels = 
{
	.nbMonster = 2,
	.monsters = {&Zombie,&Bunny}
	
};

GroupMonsters FifthLevels = 
{
	.nbMonster = 2,
	.monsters = {&Turtle,&Cyclope}
	
};
GroupMonsters SixthLevels = 
{
	.nbMonster = 2,
	.monsters = {&Lich,&Chimera}
	
};

GroupMonsters AllMonsters = 
{
	.nbMonster = 12,
	.monsters = {&Rat,&Turtle,&MagPie,&Wizard,&Zombie,&Lich,&Goblin,&Witch,&HalfZombie,&Bunny,&Cyclope,&Chimera}
	
};

uint8_t defenseLevelMap[] = 
{
	0,
	5,
	10,
	20
};

uint8_t GetMonsterDefense(uint8_t defenseLevel)
{
	if(defenseLevel < 4)
		return defenseLevelMap[defenseLevel];
	
	return 0;
}

uint16_t ScaleMonsterHealth(MonsterDef *monster, uint16_t baseHealth)
{
	switch(monster->healthLevel)
	{
		case 0:
			return baseHealth/2;
		case 2:
			return baseHealth*2;
		case 3:
			return baseHealth*2 + (uint16_t)(baseHealth/4);
		case 1:
		default:
			return baseHealth;
	}
}

uint8_t powerLevelMap[] = 
{
	10,
	7,
	4
};

uint8_t GetMonsterPowerDivisor(uint8_t powerLevel)
{
	if(powerLevel < 3)
		return powerLevelMap[powerLevel];
		
	return 10;
}

GroupMonsters *groups[] = 
{
	&FirstLevels,
	&SecondLevels,
	&ThirdLevels,
	&ForthLevels,
	&FifthLevels,
	&SixthLevels
};

MonsterDef *GetRandomMonster(uint8_t floor)
{
	uint16_t result;
	uint8_t limit;
	if(floor >= 20)
		return &Dragon;
	
	GroupMonsters *chosen;
	if(floor >= 12) {
		chosen = &AllMonsters;
	}
	else {
		limit = ((floor + 1) / 2) - 1;
		chosen = groups[limit];
	}
	
	result = Random(chosen->nbMonster) - 1;
	return chosen->monsters[result];
}


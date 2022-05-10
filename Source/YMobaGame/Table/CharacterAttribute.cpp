#include "CharacterAttribute.h"

FCharacterAttribute::FCharacterAttribute()
	:FYMobaGameTableBase()
{
	Init();
}

void FCharacterAttribute::Init()
{
	Lv = 1;
	MaxMana = 100.0f;
	Mana = MaxMana;
	MaxAnger = 0.0f;
	Anger = 0.0f;
	MaxHealth = 100.f;
	Health = MaxHealth;
	PhysicalAttack = 55.f;
	Armor = 30.f;
	MagicAttack = 0.0f;
	MagicDefense = 0.0f;
	PhysicalPenetration = 0.0f;
	MagicPenetration = 0.0f;
	MaxEmpircalValue = 100.f;
	EmpircalValue = 0.f;
	AttackSpeed = 0.66f;
	WalkSpeed = 356.f;
	RangeAttack = 1000.f;
}
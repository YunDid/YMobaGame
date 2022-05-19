// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/YMobaGameTableBase.h"
#include "CharacterAttribute.generated.h"

USTRUCT(BlueprintType)
struct FCharacterAttribute :public FYMobaGameTableBase
{
	GENERATED_USTRUCT_BODY()

public:

	FCharacterAttribute();

	//等级
	UPROPERTY()
	int32 Lv;

	//
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float MaxMana;

	UPROPERTY()
	float Mana;

	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float MaxAnger;

	UPROPERTY()
	float Anger;

	//最大生命值
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float MaxHealth;

	//生命值
	UPROPERTY()
	float Health;

	//基础攻击
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float PhysicalAttack;

	//护甲
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float Armor;

	//魔法攻击
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float MagicAttack;

	//魔法防御
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float MagicDefense;

	//物理穿透
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float PhysicalPenetration;

	//魔法穿透
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float MagicPenetration;

	//移动速度
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float WalkSpeed;

	//攻击速度
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float AttackSpeed;

	//最大经验值
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float MaxEmpircalValue;

	UPROPERTY()
	float EmpircalValue;

	//攻击范围
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float AttackRange;

public:
	void Init();

};
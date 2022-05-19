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

	//�ȼ�
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

	//�������ֵ
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float MaxHealth;

	//����ֵ
	UPROPERTY()
	float Health;

	//��������
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float PhysicalAttack;

	//����
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float Armor;

	//ħ������
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float MagicAttack;

	//ħ������
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float MagicDefense;

	//����͸
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float PhysicalPenetration;

	//ħ����͸
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float MagicPenetration;

	//�ƶ��ٶ�
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float WalkSpeed;

	//�����ٶ�
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float AttackSpeed;

	//�����ֵ
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float MaxEmpircalValue;

	UPROPERTY()
	float EmpircalValue;

	//������Χ
	UPROPERTY(EditDefaultsOnly, Category = "Character Attribute")
	float AttackRange;

public:
	void Init();

};
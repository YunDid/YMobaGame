// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
//#include "GameFramework/Pawn.h"
#include "CharacterTable.generated.h"

USTRUCT(BluePrintType)
struct FCharacterTable : public FTableRowBase 
{
	GENERATED_USTRUCT_BODY()
public:
	FCharacterTable();

	UPROPERTY(EditDefaultsOnly, Category = "Character Class")
	int64 CharacterID;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class")
	float AttackRadius;

	//Character 实例
	UPROPERTY(EditDefaultsOnly, Category = "Character Class")
	TSubclassOf<APawn> CharacterClass;

	//连击动画
	UPROPERTY(EditDefaultsOnly, Category = "Character Class")
	TArray<UAnimMontage*> CommonAttack_Animation;

	//技能动画
	UPROPERTY(EditDefaultsOnly, Category = "Character Skill Class")
	UAnimMontage* SkillAttack_Q_Animation;

	UPROPERTY(EditDefaultsOnly, Category = "Character Skill Class")
	UAnimMontage* SkillAttack_W_Animation;

	UPROPERTY(EditDefaultsOnly, Category = "Character Skill Class")
	UAnimMontage* SkillAttack_E_Animation;

	UPROPERTY(EditDefaultsOnly, Category = "Character Skill Class")
	UAnimMontage* SkillAttack_R_Animation;
};

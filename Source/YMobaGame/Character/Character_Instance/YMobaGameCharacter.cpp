// Copyright Epic Games, Inc. All Rights Reserved.

#include "YMobaGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "../../Common/MethodUnit.h"
#include "Engine/World.h"

AYMobaGameCharacter::AYMobaGameCharacter():
	bAttacking(false), Attack_Count(0), PlayerID(INDEX_NONE)
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	//测试用，PlayerID目前不知道哪儿初始化.
	PlayerID = FMath::RandRange(1000000, 8888888);

	//创建 Widget 组件.
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AYMobaGameCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	//逐帧更新玩家位置信息，用于同步.
	if (GetLocalRole() == ROLE_Authority)
	{
		if (AYMobaGameState* GameState_Ins = MethodUnit::GetYMobaGameState_Unit(GetWorld()))
		{
			GameState_Ins->UpdateCharacterAILocation(GetPlayerID(), GetActorLocation());
		}
	}

}

void AYMobaGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	//客户端模拟
	if (GetLocalRole() == ROLE_Authority) {
		SpawnDefaultController();
	}
}

void AYMobaGameCharacter::RegisterPlayerAttributes(const int64& InPlayerID, int32 CharacterID)
{
	PlayerID = InPlayerID;

	if (AYMobaGameState* GameState_Ins = MethodUnit::GetYMobaGameState_Unit(GetWorld())) {

		GameState_Ins->AddPlayerAttribute(PlayerID, CharacterID);
		GameState_Ins->AddPlayerLocation(PlayerID, GetActorLocation());
		//设置延迟时间并完成角色本身属性设置的广播.
		GetWorld()->GetTimerManager().SetTimer(InitTimeHandle, this, &AYMobaGameCharacter::InitCharacteritSelf, 0.1f);		
	}
}

void AYMobaGameCharacter::InitCharacteritSelf()
{
	//延迟初始化
	/*
		原因:
		客户端的角色是服务端同步过来的，客户端本身不创建角色，只复制副本.
		客户端的角色初始化操作必须在服务端同步之后开始.
	*/

	//
	if (InitTimeHandle.IsValid()) {	
		GetWorld()->GetTimerManager().ClearTimer(InitTimeHandle);
	}
	//获取角色配置表中的 UI_Info 后广播.
	if (AYMobaGameState* GameState_Ins = MethodUnit::GetYMobaGameState_Unit(GetWorld())) 
	{
		if (FCharacterAttribute* CharacterAttribute_Ins = GameState_Ins->GetCharacterAttributeByID(PlayerID)) {
			MutiCastWidgetInfo(CharacterAttribute_Ins->GetHealthPercentage(), CharacterAttribute_Ins->GetManaPercentage());
		}
	}

}



int64 AYMobaGameCharacter::GetPlayerID() 
{
	return PlayerID;
}

void AYMobaGameCharacter::CommonAttack(TWeakObjectPtr<AYMobaGameCharacter> Enemy)
{
	if (Enemy.IsValid()) {

		//获取PlayerID 对应的角色配置的 ID.
		int32 CharacterID = MethodUnit::GetCurrentCharacterID(GetWorld(), PlayerID);
		if (CharacterID != INDEX_NONE) {
			//获取 CharacterID 下的配置表项.
			if (const FCharacterTable* CharacterConfig = MethodUnit::GetFCharaterTableByID_Unit(GetWorld(), CharacterID)) {
				//判断当前普攻处于第几阶段.
				if (Attack_Count < CharacterConfig->CommonAttack_Animation.Num()) {

					//获取 CharacterID 下的具体普攻动画.
					if (UAnimMontage* Attack_AniMontage = CharacterConfig->CommonAttack_Animation[Attack_Count]) {
						//播放攻击动画.
						MutiCastPlayerAnimMontage(Attack_AniMontage);

						//普攻状态参数更新.
						if (Attack_Count == CharacterConfig->CommonAttack_Animation.Num() - 1) {
							//若已处于倒数第二阶段，则重置.
							Attack_Count = 0;
						}
						else {
							Attack_Count++;
						}
					}
				}
			}
		}
	}
}

UAnimMontage* AYMobaGameCharacter::GetSkillAttackAnimation(KeyCode_Type KeyCode) 
{
	//获取PlayerID 对应的角色配置的 ID.
	int32 CharacterID = MethodUnit::GetCurrentCharacterID(GetWorld(), PlayerID);
	if (CharacterID != INDEX_NONE) {
		//获取 CharacterID 下的配置表项.
		if (const FCharacterTable* CharacterConfig = MethodUnit::GetFCharaterTableByID_Unit(GetWorld(), CharacterID)) {

			//根据键的类型，返回相应的技能动画.
			switch (KeyCode) {
			case KeyCode_Type::KeyCode_Q: {
				return CharacterConfig->SkillAttack_Q_Animation;
			}; break;
			case KeyCode_Type::KeyCode_W: {
				return CharacterConfig->SkillAttack_W_Animation;
			}; break;
			case KeyCode_Type::KeyCode_E: {
				return CharacterConfig->SkillAttack_E_Animation;
			}; break;
			case KeyCode_Type::KeyCode_R: {
				return CharacterConfig->SkillAttack_R_Animation;
			}; break;
			}
		}
	}

	return nullptr;
}


void AYMobaGameCharacter::SkillAttack(KeyCode_Type KeyCode, TWeakObjectPtr<AYMobaGameCharacter> Enemy) 
{	
	//获取 KeyCode 下的具体技能动画.
	if (UAnimMontage* Attack_AniMontage = GetSkillAttackAnimation(KeyCode)) {
		//播放攻击动画.
		MutiCastPlayerAnimMontage(Attack_AniMontage);
	}
}


void AYMobaGameCharacter::MutiCastPlayerAnimMontage_Implementation(UAnimMontage* AnimMontage_Ins, float PlayRate = 1.0f, FName StartSectionName = NAME_None) 
{
	if (AnimMontage_Ins) {
		PlayAnimMontage(AnimMontage_Ins, PlayRate, StartSectionName);
	}
}

void AYMobaGameCharacter::MutiCastWidgetInfo_Implementation(float HeathPercentage, float ManaPercentage)
{
	//仅在客户端完成角色UI组件更新.
	if (GetLocalRole() != ROLE_Authority) {
		//获取当前角色的 Widget 组件转换为易操作类型后将 UI Info 参数填入.
		if (UUI_InformationBar* Widget_Ins = Cast<UUI_InformationBar>(Widget->GetUserWidgetObject())) {
			Widget_Ins->SetHealth(HeathPercentage);
			Widget_Ins->SetMana(ManaPercentage);
		}
	}
}


FCharacterAttribute* AYMobaGameCharacter::GetCurrentCharacterAttribute()
{
	if (AYMobaGameState* GameState_Ins = MethodUnit::GetYMobaGameState_Unit(GetWorld()))
	{
		return GameState_Ins->GetCharacterAttributeByID(PlayerID);
	}

	return NULL;
}

bool AYMobaGameCharacter::IsDie()
{
	if (GetCurrentCharacterAttribute()->Health <= 0.f)
	{
		return true;
	}

	return false;
}








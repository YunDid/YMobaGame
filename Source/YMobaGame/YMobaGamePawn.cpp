// Fill out your copyright notice in the Description page of Project Settings.


#include "YMobaGamePawn.h"
#include "Character/Character_Instance/YMobaGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Common/MethodUnit.h"
#include "Character/AI/AIController/YMobaGameAIController.h"
#include "Engine/World.h"

// Sets default values
AYMobaGamePawn::AYMobaGamePawn()
{

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//��������Ͷ������
	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RootBox"));
	RootComponent = RootBox;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

// Called when the game starts or when spawned
void AYMobaGamePawn::BeginPlay()
{
	Super::BeginPlay();
	//���ڷ��������� Charactor ʵ��
	if (GetLocalRole() == ROLE_Authority) {

		//��ȡ GameState ʵ��
		AYMobaGameState* YMobaGameState =  MethodUnit::GetYMobaGameState_Unit(GetWorld());

		if (YMobaGameState) {

			//�ⲿ�ļ���ȡ CharacterID
			FString StringID;
			FFileHelper::LoadFileToString(StringID,*(FPaths::ProjectDir() / TEXT("CharacterID.txt")));
			int64 CharaterID = FCString::Atoi64(*StringID);

			if (const FCharacterTable* FCharacterTable_Ins = YMobaGameState->GetFCharaterTableByID(CharaterID)) {
				DefaultPawnClass = FCharacterTable_Ins->CharacterClass;
			}

			//���� Charactor ʵ��
			if (DefaultPawnClass) {
				MobaGameCharacter = GetWorld()->SpawnActor<AYMobaGameCharacter>(DefaultPawnClass, GetActorLocation(), GetActorRotation());
				if (MobaGameCharacter) {
					//��ʼ�� Character �µ� CharacterID.
					MobaGameCharacter->InitCharacterID(CharaterID);
				}
			}
		}
		
	}
	
}

void AYMobaGamePawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//�ڷ�������ɿͻ����˳�ʱ�����٣�����ͻ����Զ�ͬ��
	if (GetLocalRole() == ROLE_Authority) {
		if (MobaGameCharacter) {
			MobaGameCharacter->Destroy();
		}
	}
}

// Called every frame
void AYMobaGamePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		if (CursorToWorld != nullptr) {

			//�ͻ�����������֡������ʾ������˽���Ҫ���ܿͻ��˴��������ݼ������ݼ���
			if (APlayerController* PC = Cast<APlayerController>(GetController()))
			{
				//���λ������
				FHitResult TraceHitResult;
				PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
				FVector CursorFV = TraceHitResult.ImpactNormal;
				FRotator CursorR = CursorFV.Rotation();
				CursorToWorld->SetWorldLocation(TraceHitResult.Location);
				CursorToWorld->SetWorldRotation(CursorR);
			}
		}
	}

}

// Called to bind functionality to input
void AYMobaGamePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AYMobaGamePawn::CharactorMoveToOnServer_Implementation(const FVector& DirectionLocation)
{
	if (MobaGameCharacter)
	{		
		//��ȡ�����λ�����ɫλ�õľ���
		float const Distance = FVector::Dist(DirectionLocation, MobaGameCharacter->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{			
			//���� AIController ���� AI ��Ϊ�����ƵĽ�ɫ�ƶ�������ά��.
			AYMobaGameAIController* AIController = Cast<AYMobaGameAIController>(MobaGameCharacter->GetController());
			if (AIController) {
				AIController->SimpleMoverTo(DirectionLocation);
			}

			//���� Pawn ����ͼ���ƵĽ�ɫ�ƶ���������ά��.
			//UAIBlueprintHelperLibrary::SimpleMoveToLocation(MobaGameCharacter->GetController(), DirectionLocation);
		}
	}
}

void AYMobaGamePawn::MoveToEnemyAndAttackOnServer_Implementation(const FVector& DirectionLocation, const APawn* Enemy)
{
	if (MobaGameCharacter) {
		if (AYMobaGameAIController* AIController = Cast<AYMobaGameAIController>(MobaGameCharacter->GetController())) {
			AIController->SetAttackTarget(Cast<AYMobaGameCharacter>(const_cast<APawn*> (Enemy)));
		}
	}
	
}

void AYMobaGamePawn::SkillAttack(KeyCode_Type KeyCode, const APawn* Enemy) {

	//���� MobaGameCharacter �ײ㼼�ܶ����߼�.
	if (MobaGameCharacter) {
		MobaGameCharacter->SkillAttack(KeyCode, Cast<AYMobaGameCharacter>(const_cast<APawn*>(Enemy)));
	}

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "MobaPawn.h"
#include "YMobaGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/World.h"

// Sets default values
AMobaPawn::AMobaPawn()
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

	//解决摄像机投射问题
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
void AMobaPawn::BeginPlay()
{
	Super::BeginPlay();
	//仅在服务器生成 Charactor 实例
	if (GetLocalRole() == ROLE_Authority) {
		//生成 Charactor 实例
		if (DefaultPawnClass) {
			MobaGameCharacter = GetWorld()->SpawnActor<AYMobaGameCharacter>(DefaultPawnClass, GetActorLocation(), GetActorRotation());
		}
	}
	
}

void AMobaPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//于服务器完成客户端退出时的销毁，其余客户端自动同步
	if (GetLocalRole() == ROLE_Authority) {
		if (MobaGameCharacter) {
			MobaGameCharacter->Destroy();
		}
	}
}

// Called every frame
void AMobaPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		if (CursorToWorld != nullptr) {

			//客户端作光标的逐帧更新显示，服务端仅需要接受客户端传来的数据即可数据即可
			if (APlayerController* PC = Cast<APlayerController>(GetController()))
			{
				//光标位置设置
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
void AMobaPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMobaPawn::CharactorMoveToOnServer_Implementation(const FVector& DirectionLocation)
{
	if (MobaGameCharacter)
	{
		//获取鼠标点击位置与角色位置的距离
		float const Distance = FVector::Dist(DirectionLocation, MobaGameCharacter->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(MobaGameCharacter->GetController(), DirectionLocation);
		}
	}
}


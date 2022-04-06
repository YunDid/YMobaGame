// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MobaGameState.h"
#include "MobaPawn.generated.h"

class AYMobaGameCharacter;

UCLASS()
class YMOBAGAME_API AMobaPawn : public APawn
{
	GENERATED_BODY()

public:
	//上层管理摄像机与游标，下层 Charactor 管理角色逻辑
	//这里复用一个 DefaultPawnClass 用作角色的逻辑控制 Pawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<APawn> DefaultPawnClass;

public:
	// Sets default values for this pawn's properties
	AMobaPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* CursorToWorld;

	/** Root Component. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* RootBox;

public:
	//TCP + 专属服务器
	UFUNCTION(server, reliable)
		void CharactorMoveToOnServer(const FVector& DirectionLocation);
protected:
	//用于角色逻辑控制的 Character
	/*
		目的: 实现 摄像机+游标逻辑 与 角色逻辑 控制的解耦
		方式: 1. AMobaPawn 控制 摄像机+游标逻辑
			  2. MobaGameCharacter 控制 角色逻辑
			  3. AMobaPawn 复用 MobaGameCharacter 实现角色的实例化
	*/
	AYMobaGameCharacter* MobaGameCharacter;
};

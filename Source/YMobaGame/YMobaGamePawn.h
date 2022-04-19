// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "YMobaGameEnums.h"
#include "YMobaGameState.h"
#include "YMobaGamePawn.generated.h"

class AYMobaGameCharacter;

UCLASS()
class YMOBAGAME_API AYMobaGamePawn : public APawn
{
	GENERATED_BODY()

public:
	//�ϲ������������α꣬�²� Charactor �����ɫ�߼�
	//���︴��һ�� DefaultPawnClass ������ɫ���߼����� Pawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<APawn> DefaultPawnClass;

public:
	// Sets default values for this pawn's properties
	AYMobaGamePawn();

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
	//TCP + ר��������
	UFUNCTION(server, reliable)
	void CharactorMoveToOnServer(const FVector& DirectionLocation);

	UFUNCTION(server, reliable)
	void MoveToEnemyAndAttackOnServer(const FVector& DirectionLocation, const APawn* Enemy);

	void SkillAttack(KeyCode_Type KeyCode, const APawn* Enemy);

protected:
	//���ڽ�ɫ�߼����Ƶ� Character
	/*
		Ŀ��: ʵ�� �����+�α��߼� �� ��ɫ�߼� ���ƵĽ���
		��ʽ: 1. AYMobaGamePawn ���� �����+�α��߼�
			  2. MobaGameCharacter ���� ��ɫ�߼�
			  3. AYMobaGamePawn ���� MobaGameCharacter ʵ�ֽ�ɫ��ʵ����
	*/
	AYMobaGameCharacter* MobaGameCharacter;
};

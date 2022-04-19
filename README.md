# YMobaGame

> ����������Ŀ��������¼�ճ������ܽᣬ�����޷����֣�������.
>
> ����Ŀ������ ue4.25��TopDown ģ�壬Demo�ںϿͻ��˺ͷ���˼�Ⱥ����ģ�飬ʵ�� Moba ������Ϸ�ֲ�ʽ����.

# Getting up and running / ��Ŀ����
> The steps below will take you through cloning your own private fork, then compiling and running the editor yourself:

## Windows for Ue4.25  / �������
- Install GitHub for Windows then fork and clone our repository. To use Git from the command line, see the Setting up Git and Fork a Repo articles.

> If you'd prefer not to use Git, you can get the source with the 'Download ZIP' button on the right. The built-in Windows zip utility will mark the contents of zip files downloaded from the Internet as unsafe to execute, so right-click the zip file and select 'Properties...' and 'Unblock' before decompressing it. Third-party zip utilities don't normally do this.

- Install Visual Studio 2019. All desktop editions of Visual Studio 2019 can build UE4, including Visual Studio Community 2019, which is free for small teams and individual developers. To install the correct components for UE4 development, check the "Game Development with C++" workload, and the "Unreal Engine Installer" and "Nuget Package Manager" optional components.

- Open your source folder in Explorer and run Setup.bat. This will download binary content for the engine, as well as installing prerequisites and setting up Unreal file associations. On Windows 8, a warning from SmartScreen may appear. Click "More info", then "Run anyway" to continue.

> A clean download of the engine binaries is currently 3-4gb, which may take some time to complete. Subsequent checkouts only require incremental downloads and will be much quicker.

- Run GenerateProjectFiles.bat to create project files for the engine. It should take less than a minute to complete.

- Load the project into Visual Studio by double-clicking on the UE4.sln file. Set your solution configuration to Development Editor and your solution platform to Win64, then right click on the UE4 target and select Build. It may take anywhere between 10 and 40 minutes to finish compiling, depending on your system specs.

- After compiling finishes, you can load the editor from Visual Studio by setting your startup project to UE4 and pressing F5 to debug.

`ע��:`

1. ����Ҫ���� Win64 ƽ̨�µ��������û������ֱ�Ϊ DebugGame Editor��DebugGame Server��DebugGame.
2. ����UE4Ϊ������Ŀ���� DebugGame Editor ����������. 

## Windows for Project / ��Ŀ����

- �� TopDownCPP Ϊģ�崴�� C++ ��Ŀ.
- �� Programs �µ����� c# �ļ�(�����ļ���)��Ŀ��������Ϊ .NET Framework 4.6.2.
- ���� Win64 ƽ̨�µ� DebugGame Editor ���û���������.

# Except for missing you. / ��������

# What have I done.

## 1. Decoupling control / ���ƽ���

> TopDown ģ���µĽ�ɫ���ƣ������˰���ϵ�еĿ��ƣ�������������ɫһ���ƶ��������Ⲣ���� Moba ��Ϸ�������ĳ���.

1. ��ԭ�� Charater �����ϣ������ϳ����һ�� YMobaGamePawn�����������������α�.
2. ��ɫ���߼�����ͨ�� YMobaGamePawn ���� һ�� Charater ��ʵ��.

```c++ 
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
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
    
	//TCP + ר��������
	UFUNCTION(server, reliable)
		void CharactorMoveToOnServer(const FVector& DirectionLocation);
public:
    
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

```

## 2. Mobile Character / ��ɫ�ƶ�

> Moba ��Ҫ���������λ�ã����ʵʱ��ɫ���ƶ�.

1. YMobaGamePawn ������ʾ�ɱ����Ƶĸ���ṩ�������ƶ���ڣ��� PlayerControler ��ɿ���.
2. YMobaGamePawn �� MobaGameCharacter �ṩ�ƶ��ӿ�
3. ��ȡ�����λ�����ɫλ�õľ���.
4. \>120.0f ��ͨ�� SimpleMoveToLocation(...)��ɽ�ɫ���ƶ�.
5. ���򲻷����ƶ�.

`ע��:`

1. ��ɫ�ƶ��߼���Ҫ���������֤���ɷ������ʵ��.
2. �α�UI��λ�ø����ɿͻ��������.
3. ��Ļ���������ƶ��ɲ���ڿͻ������.
4. Pawn ����ͼ���ƵĽ�ɫ�ƶ��ӿڽ����ã���ɫ�ƶ��߼�������Ϊ��ͳһ����.

## 3. Packaging / ���

> ʵ�� aliyun �� Linux ��˫�˲���.

### aliyun / �����Ʋ���

1. aliyun ECS��������¶ UDP:7777 �˿�.
2. Ue ������ Package Setting �µĿͻ���Ĭ�ϵ�ͼΪ Login�������Ĭ�ϵ�ͼΪ TopDown.
3. ���� Server.Target.cs �ļ�.
4. �� Login �� TopDown ��� Ue �� Client �� Server �Ĵ��.
5. Server ���� bat �ļ�ָ�� Win64 DebugGame Server ���������ɵ� DebugGameServer.exe.
6. �� Server �������� aliyun ECS ��������.
7. Client ͨ�� aliyun ECS �Ĺ��� IP:7777 ��ɶԷ�������������ʣ�����ͨ�����紩͸����.

### Linux / ����

1. �������� Linux ��Ӧ Ue �汾�Ľ�����빤����.
2. ���� LINUX_MULTIARCH_ROOT ��������Ϊ clang ���߰�װ·��.
3. ���±��� Ue �� AutomationTool��UnrealBuildTool��UnrealFrontend ģ��.
4. ���±�����Ŀ�� Linux �µ� DebugGame Server ���û���.
5. ���� Server.Target.cs �ļ�.
6. ���  Ue �¶� TopDown ��ͼ�� LinuxServer �Ĵ��.
7. �� LinuxServer ������ Linux ����.
8. Client ͨ�� Linux �ľ����� IP:7777 ��ɶԷ��������������.

## 4. Configuration Table / ���ñ�

> Ue �´��������ݱ��Ϊ UDataTable ���ͣ��ñ���µ�ÿ�е���Ŀ����Ϊ FTableRowBase ����.

1. ���� FTableRowBase ���������õ���Ŀ������ CharacterID �� Character ��ͼʵ�����ͣ�FCharacterTable.

2. �� YMobaGameState ��� UDataTable ���ݱ��� FCharacterTable ��Ŀ���ݵĴ洢�����ͨ��ID���ʽӿڵı�¶.

   > ���ñ������� TArray ��ʽ����� YMobaGameState ��
   >
   > - YMobaGameState ��Ҫ���� static ConstructorHelpers::FObjectFinder\<UDataTable\> ��ȡ���� UDataTable ���ݱ��
   > - ��ȡ�� UDataTable ���ݱ���µ����� FCharacterTable ������Ŀ���� TArray ���ݴ洢����.

3. YMobaGamePawn ͨ�� YMobaGameState ��¶�Ľӿڣ���ɶԽ�ɫ���õĻ�ȡ����������ʵ�ʵ� DefaultPawn.

`ע��:`

1. ͨ�� CharacterID ��ȡ ��ɫ����ʱ����ͨ��������ȡ FCharaterTable_Cache���ٱ�������Ҷ�ӦID�µĽ�ɫ����.
2. YMobaGamePawn ʹ�õ� ID ��ʹ���ⲿ�ļ����ã���������.

**Questions:** 

- DefaultPawn �� YMobaGamePawn ��ʲô��ϵ��

## 5. Attack Logic / �����߼�

> �������ͨ����ͨ�����е����߼�⣬��ȡ��ײ��Ӧ�Ķ���.
>
> ��Ϊ APawn ��� Enemy���򽫸� Enemy ��������Ϊ���ڰ��У�������Ϊ�������µ��չ��߼�.
>
> ���򣬽��������λ��ת��Ϊ���������������ڰ��У������ƶ���Ϊ�������µ��ƶ��߼�.

`ע��:`

1. ��ͬ��ɫ�в�ͬ�Ĺ���Ч�������ǽ�ʹ��ͬһ�������.

2. Moba ���ɫ����Ϊ��ʹ�� Behaviour Tree / ��Ϊ�� ��ͳһ��Ϊ���.

### Common Attack /  ��ͨ����

> �㲥��Ӧ�����׶��µ���̫�涯��.

`ע��:`

1. ��ɫ������ n ���չ�����˽�ɫ������һ�� Attack_Count ����¼��ǰ���������Ľ׶Σ�ͨ�� Attack_Count ��������Ӧ�׶ε� CommonAttack_Animation[i] �����Ĺ㲥.
2. ��ͬ�Ľ�ɫ�����в�ͬ���������ԣ���˽�ɫ������ CharacterID ����ȡ��Ӧ���ñ��еĹ����������ԣ��綯����̫��.

```c++ 
void AYMobaGameCharacter::CommonAttack(TWeakObjectPtr<AYMobaGameCharacter> Enemy)
{
	if (Enemy.IsValid()) {
		//��ȡ CharacterID �µ����ñ���.
		if (const FCharacterTable* CharacterConfig =  MethodUnit::GetFCharaterTableByID_Unit(GetWorld(), CharacterID)) {
			//�жϵ�ǰ�չ����ڵڼ��׶�.
			if (Attack_Count < CharacterConfig->CommonAttack_Animation.Num()) {

				//��ȡ CharacterID �µľ����չ�����.
				if (UAnimMontage* Attack_AniMontage = CharacterConfig->CommonAttack_Animation[Attack_Count]) {
					//���Ź�������.
					MutiCastPlayerAnimMontage(Attack_AniMontage);

					//�չ�״̬��������.
					if (Attack_Count == CharacterConfig->CommonAttack_Animation.Num() - 1) {
						//���Ѵ��ڵ����ڶ��׶Σ�������.
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
```

#### Flow / ������

> UYMobaGameAttack_BTTaskNode::ExecuteTask() -> AYMobaGameAIController::CommonAttack() -> AYMobaGameCharacter::CommonAttack()
>
> �չ��߼�������Ϊ���µ�����ڵ��б�ִ�У�����ڵ㽫��ȡ AI Controller �µĹ����ӿڣ�����ת�� Character �µĹ����߻��߼�.

### Skill Attack / ���ܹ���

> ������Ӧ�� Keycode_Type �������ͣ���ȡ���ñ�����Ӧ�ļ�����̫�涯������������й㲥.

`ע��:`

1. QWER ����Ϊ����ӳ�䣬��Ӧ���¼�����Ӧ����Ϊ server ���ԣ��ɷ����������.
2. Keycode_Type ö������ӦΪ enum class ��Ӧ�̳��� uint8.

```c++ 
void AYMobaGameCharacter::SkillAttack(KeyCode_Type KeyCode, TWeakObjectPtr<AYMobaGameCharacter> Enemy) 
{	
	//��ȡ KeyCode �µľ��弼�ܶ���.
	if (UAnimMontage* Attack_AniMontage = GetSkillAttackAnimation(KeyCode)) {
		//���Ź�������.
		MutiCastPlayerAnimMontage(Attack_AniMontage);
	}
}
```

## Collision Overview / ��ײ����

> `Ue Document: Collision Responses and Trace Responses form the basis for how Unreal Engine 4 handles collision and ray casting during run time.`
>
> Collision Responses / ��ײ��Ӧ �� Trace Responses / ׷����Ӧ ������ Ue ������ײ�͹���Ͷ��Ļ���.
>
> `Ue Document: Every object that can collide gets an Object Type and a series of responses that define how it interacts with all other object types.`
>
> ÿ���������Լ��� Object Type���Լ�һϵ�е���Ӧ����Щ��Ӧ������������������()�Ľ�����ʽ.
>
> `Ue Document: When a collision or overlap event occurs, both (or all) objects involved can be set to affect or be affected by blocking, overlapping, or ignoring each other.`
>
> blocking / �赲��overlapping / �ص���ignoring / ���ԣ�������Ӧ��ʽ�������˽Ӵ�����Ļ���Ӱ������Ӱ��ķ�ʽ.

#### Collision Responses / ��ײ��Ӧ

> ``Ue Document: These define how this Physics Body should interact with all of the other types of Trace and Object Types. Remember, it's the interaction between both Physics Bodies that define the resulting action, so the Object Type and Collision Responses of both Physics Bodies matter.`
>
> ��ײ��Ӧ�����˴�������������������׷��������������͵Ľ�����ʽ.
>
> `ע��:`
>
> 1. ���յĽ���Ч���������ߵ���Ӧ���ù�ͬ�����������¹���.

#### Trace Responses / ׷����Ӧ

> `Ue Document: Trace responses are used in Traces (ray casts), such as the Blueprint Node Line Trace by Channel.`
>
> �켣׷����Ӧ��Ӧ���ڹ���Ͷ���⣬���簴�ŵ��켣׷��.

- FHitResult

  > `Ue Document: Structure containing information about one hit of a trace, such as point of impact and surface normal at that point.`
  >
  > FHitResult �ṹ������˹켣���е���Ϣ.

- HitResult.bBlockingHit

  > ` Ue Document: Indicates if this hit was a result of blocking collision.`
  >
  > HitResult.bBlockingHit �������н���Ƿ�Ϊһ���赲��ײ.

- ECollisionChannel / ��ײ���

  > ���� ECC_GameTraceChannel1 �����Զ���Ĺ��������׷��.
  >
  > `ע��:`
  >
  > 1. ECC_TraceName ��Ҫ�� DefaultEngine.ini �ļ��� [/Script/Engine.CollisionProfile] ��Ŀ�л�ȡ.

```c++ 
FHitResult HitResult;
FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(ClickableTrace), false);
//���� ECC_GameTraceChannel1 �����׷��
if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldOrigin, WorldOrigin + WorldDirection * HitResultTraceDistance, ECC_GameTraceChannel1, CollisionQueryParams)) {
if (HitResult.bBlockingHit) {
	//��⵽���ƶ�����
	MyPawn->MoveToEnemyAndAttackOnServer(HitResult.ImpactPoint, Cast<APawn>(HitResult.Actor));
	}
}
```



## Behaviour Tree / ��Ϊ��

### BlackBorad / �ڰ�

### BT - Composition Node / ���Ͻڵ�

### BT - Service Node / ����ڵ�

### BT - Task Node / ����ڵ�

### AI Controller / AI ������
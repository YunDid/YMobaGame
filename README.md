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

1. ��ԭ�� Charater �����ϣ������ϳ����һ�� MobaPawn�����������������α�.
2. ��ɫ���߼�����ͨ�� MobaPawn ���� һ�� Charater ��ʵ��.

```c++ 
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
	//�ϲ������������α꣬�²� Charactor �����ɫ�߼�
	//���︴��һ�� DefaultPawnClass ������ɫ���߼����� Pawn
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
		��ʽ: 1. AMobaPawn ���� �����+�α��߼�
			  2. MobaGameCharacter ���� ��ɫ�߼�
			  3. AMobaPawn ���� MobaGameCharacter ʵ�ֽ�ɫ��ʵ����
	*/
	AYMobaGameCharacter* MobaGameCharacter;
};

```

## 2. Mobile Character / ��ɫ�ƶ�

> Moba ��Ҫ���������λ�ã����ʵʱ��ɫ���ƶ�.

1. MobaPawn ������ʾ�ɱ����Ƶĸ���ṩ�������ƶ���ڣ��� PlayerControler ��ɿ���.
2. MobaPawn �� MobaGameCharacter �ṩ�ƶ��ӿ�
3. ��ȡ�����λ�����ɫλ�õľ���.
4. \>120.0f ��ͨ�� SimpleMoveToLocation(...)��ɽ�ɫ���ƶ�.
5. ���򲻷����ƶ�.

`ע��:`

1. ��ɫ�ƶ��߼���Ҫ���������֤���ɷ������ʵ��.
2. �α�UI��λ�ø����ɿͻ��������.
3. ��Ļ���������ƶ��ɲ���ڿͻ������.

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

## Configuration Table / ���ñ�

> Ue �´��������ݱ��Ϊ UDataTable ���ͣ��ñ���µ�ÿ�е���Ŀ����Ϊ FTableRowBase ����.

1. ���� FTableRowBase ���������õ���Ŀ������ CharacterID �� Character ��ͼʵ�����ͣ�FCharacterTable.

2. �� MobaGameState ��� UDataTable ���ݱ��� FCharacterTable ��Ŀ���ݵĴ洢�����ͨ��ID���ʽӿڵı�¶.

   > ���ñ����ݴ������TArray��ʽ MobaGameState ��
   >
   > - MobaGameState ��Ҫ���� static ConstructorHelpers::FObjectFinder\<UDataTable\> ��ȡ���� UDataTable ���ݱ��
   > - ��ȡ�� UDataTable ���ݱ���µ����� FCharacterTable ������Ŀ���� TArray ���ݴ洢����.

3. MobaPawn ͨ�� MobaGameState ��¶�Ľӿڣ���ɶԽ�ɫ���õĻ�ȡ����������ʵ�ʵ� DefaultPawn.

`ע��:`

1. ͨ�� CharacterID ��ȡ ��ɫ����ʱ����ͨ��������ȡ FCharaterTable_Cache���ٱ�������Ҷ�ӦID�µĽ�ɫ����.
2. MobaPawn ʹ�õ� ID ��ʹ���ⲿ�ļ����ã���������.

**Questions:** 

- DefaultPawn �� MobaPawn ��ʲô��ϵ��

## Attack Logic / �����߼�


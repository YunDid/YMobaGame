# YMobaGame

> 个人娱乐项目，将仅记录日常进度总结，您若无法重现，望见谅.
>
> 此项目将基于 ue4.25，TopDown 模板，Demo融合客户端和服务端集群量大模块，实现 Moba 网络游戏分布式部署.

# Getting up and running / 项目启动
> The steps below will take you through cloning your own private fork, then compiling and running the editor yourself:

## Windows for Ue4.25  / 引擎编译
- Install GitHub for Windows then fork and clone our repository. To use Git from the command line, see the Setting up Git and Fork a Repo articles.

> If you'd prefer not to use Git, you can get the source with the 'Download ZIP' button on the right. The built-in Windows zip utility will mark the contents of zip files downloaded from the Internet as unsafe to execute, so right-click the zip file and select 'Properties...' and 'Unblock' before decompressing it. Third-party zip utilities don't normally do this.

- Install Visual Studio 2019. All desktop editions of Visual Studio 2019 can build UE4, including Visual Studio Community 2019, which is free for small teams and individual developers. To install the correct components for UE4 development, check the "Game Development with C++" workload, and the "Unreal Engine Installer" and "Nuget Package Manager" optional components.

- Open your source folder in Explorer and run Setup.bat. This will download binary content for the engine, as well as installing prerequisites and setting up Unreal file associations. On Windows 8, a warning from SmartScreen may appear. Click "More info", then "Run anyway" to continue.

> A clean download of the engine binaries is currently 3-4gb, which may take some time to complete. Subsequent checkouts only require incremental downloads and will be much quicker.

- Run GenerateProjectFiles.bat to create project files for the engine. It should take less than a minute to complete.

- Load the project into Visual Studio by double-clicking on the UE4.sln file. Set your solution configuration to Development Editor and your solution platform to Win64, then right click on the UE4 target and select Build. It may take anywhere between 10 and 40 minutes to finish compiling, depending on your system specs.

- After compiling finishes, you can load the editor from Visual Studio by setting your startup project to UE4 and pressing F5 to debug.

`注意:`

1. 你需要编译 Win64 平台下的三个配置环境，分别为 DebugGame Editor，DebugGame Server，DebugGame.
2. 设置UE4为启动项目，在 DebugGame Editor 配置下启动. 

## Windows for Project / 项目创建

- 以 TopDownCPP 为模板创建 C++ 项目.
- 将 Programs 下的所有 c# 文件(含子文件夹)的目标框架设置为 .NET Framework 4.6.2.
- 编译 Win64 平台下的 DebugGame Editor 配置环境，启动.

# Except for missing you. / 除了想你

# What have I done.

## 1. Decoupling control / 控制解耦

> TopDown 模板下的角色控制，采用了暗黑系列的控制，摄像机将跟随角色一并移动，但是这并不是 Moba 游戏所期望的场景.

1. 在原先 Charater 基础上，再向上抽离出一个 YMobaGamePawn，负责管理摄像机与游标.
2. 角色的逻辑控制通过 YMobaGamePawn 复用 一个 Charater 来实现.

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
	//上层管理摄像机与游标，下层 Charactor 管理角色逻辑
	//这里复用一个 DefaultPawnClass 用作角色的逻辑控制 Pawn
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
    
	//TCP + 专属服务器
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
	//用于角色逻辑控制的 Character
	/*
		目的: 实现 摄像机+游标逻辑 与 角色逻辑 控制的解耦
		方式: 1. AYMobaGamePawn 控制 摄像机+游标逻辑
			  2. MobaGameCharacter 控制 角色逻辑
			  3. AYMobaGamePawn 复用 MobaGameCharacter 实现角色的实例化
	*/
	AYMobaGameCharacter* MobaGameCharacter;
};

```

## 2. Mobile Character / 角色移动

> Moba 需要根据鼠标点击位置，完成实时角色的移动.

1. YMobaGamePawn 仅仅表示可被控制的概念，提供基本的移动借口，由 PlayerControler 完成控制.
2. YMobaGamePawn 下 MobaGameCharacter 提供移动接口
3. 获取鼠标点击位置与角色位置的距离.
4. \>120.0f 则通过 SimpleMoveToLocation(...)完成角色的移动.
5. 否则不发生移动.

`注意:`

1. 角色移动逻辑需要服务端来验证并由服务端来实现.
2. 游标UI的位置更新由客户端来完成.
3. 屏幕跟随鼠标的移动由插件于客户端完成.
4. Pawn 由蓝图控制的角色移动接口将弃用，角色移动逻辑将由行为树统一管理.

## 3. Packaging / 打包

> 实现 aliyun 和 Linux 的双端部署.

### aliyun / 阿里云部署

1. aliyun ECS服务器暴露 UDP:7777 端口.
2. Ue 下设置 Package Setting 下的客户端默认地图为 Login，服务端默认地图为 TopDown.
3. 创建 Server.Target.cs 文件.
4. 将 Login 与 TopDown 完成 Ue 下 Client 与 Server 的打包.
5. Server 创建 bat 文件指向 Win64 DebugGame Server 配置下生成的 DebugGameServer.exe.
6. 将 Server 包部署于 aliyun ECS 服务器下.
7. Client 通过 aliyun ECS 的公网 IP:7777 完成对服务器的请求访问，或者通过网络穿透访问.

### Linux / 部署

1. 官网下载 Linux 对应 Ue 版本的交叉编译工具链.
2. 配置 LINUX_MULTIARCH_ROOT 环境变量为 clang 工具安装路径.
3. 重新编译 Ue 下 AutomationTool，UnrealBuildTool，UnrealFrontend 模块.
4. 重新编译项目在 Linux 下的 DebugGame Server 配置环境.
5. 创建 Server.Target.cs 文件.
6. 完成  Ue 下对 TopDown 地图的 LinuxServer 的打包.
7. 将 LinuxServer 部署于 Linux 即可.
8. Client 通过 Linux 的局域网 IP:7777 完成对服务器的请求访问.

## 4. Configuration Table / 配置表

> Ue 下创建的数据表格为 UDataTable 类型，该表格下的每行的条目数据为 FTableRowBase 类型.

1. 基于 FTableRowBase 创建可配置的条目，例如 CharacterID 与 Character 蓝图实例类型，FCharacterTable.

2. 由 YMobaGameState 完成 UDataTable 数据表下 FCharacterTable 条目数据的存储，与可通过ID访问接口的暴露.

   > 配置表数据以 TArray 形式存放于 YMobaGameState 下
   >
   > - YMobaGameState 需要先由 static ConstructorHelpers::FObjectFinder\<UDataTable\> 获取到的 UDataTable 数据表格
   > - 获取该 UDataTable 数据表格下的所有 FCharacterTable 数据条目，以 TArray 数据存储起来.

3. YMobaGamePawn 通过 YMobaGameState 暴露的接口，完成对角色配置的获取，进而设置实际的 DefaultPawn.

`注意:`

1. 通过 CharacterID 获取 角色类型时，先通过单例获取 FCharaterTable_Cache，再遍历其查找对应ID下的角色类型.
2. YMobaGamePawn 使用的 ID 将使用外部文件设置，方便配置.

**Questions:** 

- DefaultPawn 与 YMobaGamePawn 是什么关系？

## 5. Attack Logic / 攻击逻辑

> 鼠标点击后，通过按通道进行的射线检测，获取碰撞响应的对象.
>
> 若为 APawn 类的 Enemy，则将该 Enemy 设置至行为树黑板中，触发行为树管理下的普攻逻辑.
>
> 否则，将鼠标点击的位置转换为世界坐标设置至黑板中，触发移动行为树管理下的移动逻辑.

`注意:`

1. 不同角色有不同的攻击效果，但是将使用同一攻击框架.

2. Moba 类角色的行为将使用 Behaviour Tree / 行为树 作统一行为框架.

### Common Attack /  普通攻击

> 广播对应攻击阶段下的蒙太奇动画.

`注意:`

1. 角色将具有 n 段普攻，因此角色将内置一个 Attack_Count 来记录当前攻击所处的阶段，通过 Attack_Count 来进行相应阶段的 CommonAttack_Animation[i] 动画的广播.
2. 不同的角色将具有不同的配置属性，因此角色将内置 CharacterID 来获取相应配置表中的攻击配置属性，如动画蒙太奇.

```c++ 
void AYMobaGameCharacter::CommonAttack(TWeakObjectPtr<AYMobaGameCharacter> Enemy)
{
	if (Enemy.IsValid()) {
		//获取 CharacterID 下的配置表项.
		if (const FCharacterTable* CharacterConfig =  MethodUnit::GetFCharaterTableByID_Unit(GetWorld(), CharacterID)) {
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
```

#### Flow / 调用流

> UYMobaGameAttack_BTTaskNode::ExecuteTask() -> AYMobaGameAIController::CommonAttack() -> AYMobaGameCharacter::CommonAttack()
>
> 普攻逻辑将于行为树下的任务节点中被执行，任务节点将获取 AI Controller 下的攻击接口，进而转调 Character 下的攻击具化逻辑.

### Skill Attack / 技能攻击

> 根据相应的 Keycode_Type 按键类型，获取配置表中相应的技能蒙太奇动画，并对其进行广播.

`注意:`

1. QWER 技能为操作映射，相应的事件函数应设置为 server 属性，由服务端来处理.
2. Keycode_Type 枚举类型应为 enum class 且应继承自 uint8.

```c++ 
void AYMobaGameCharacter::SkillAttack(KeyCode_Type KeyCode, TWeakObjectPtr<AYMobaGameCharacter> Enemy) 
{	
	//获取 KeyCode 下的具体技能动画.
	if (UAnimMontage* Attack_AniMontage = GetSkillAttackAnimation(KeyCode)) {
		//播放攻击动画.
		MutiCastPlayerAnimMontage(Attack_AniMontage);
	}
}
```

## Collision Overview / 碰撞概述

> `Ue Document: Collision Responses and Trace Responses form the basis for how Unreal Engine 4 handles collision and ray casting during run time.`
>
> Collision Responses / 碰撞响应 与 Trace Responses / 追踪响应 构成了 Ue 处理碰撞和光线投射的基础.
>
> `Ue Document: Every object that can collide gets an Object Type and a series of responses that define how it interacts with all other object types.`
>
> 每个对象将有自己的 Object Type，以及一系列的响应，这些响应决定了其与其他对象()的交互方式.
>
> `Ue Document: When a collision or overlap event occurs, both (or all) objects involved can be set to affect or be affected by blocking, overlapping, or ignoring each other.`
>
> blocking / 阻挡，overlapping / 重叠，ignoring / 忽略，三种响应方式，决定了接触对象的互相影响与受影响的方式.

#### Collision Responses / 碰撞响应

> ``Ue Document: These define how this Physics Body should interact with all of the other types of Trace and Object Types. Remember, it's the interaction between both Physics Bodies that define the resulting action, so the Object Type and Collision Responses of both Physics Bodies matter.`
>
> 碰撞响应定义了此物理形体与所有其他追踪类型与对象类型的交互方式.
>
> `注意:`
>
> 1. 最终的交互效果将由两者的响应设置共同绝决定，向下规整.

#### Trace Responses / 追踪响应

> `Ue Document: Trace responses are used in Traces (ray casts), such as the Blueprint Node Line Trace by Channel.`
>
> 轨迹追踪响应将应用于光线投射检测，例如按信道轨迹追踪.

- FHitResult

  > `Ue Document: Structure containing information about one hit of a trace, such as point of impact and surface normal at that point.`
  >
  > FHitResult 结构体包含了轨迹命中的信息.

- HitResult.bBlockingHit

  > ` Ue Document: Indicates if this hit was a result of blocking collision.`
  >
  > HitResult.bBlockingHit 表明命中结果是否为一个阻挡碰撞.

- ECollisionChannel / 碰撞轨道

  > 例如 ECC_GameTraceChannel1 这类自定义的轨道，用于追踪.
  >
  > `注意:`
  >
  > 1. ECC_TraceName 需要在 DefaultEngine.ini 文件中 [/Script/Engine.CollisionProfile] 栏目中获取.

```c++ 
FHitResult HitResult;
FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(ClickableTrace), false);
//作对 ECC_GameTraceChannel1 轨道的追踪
if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldOrigin, WorldOrigin + WorldDirection * HitResultTraceDistance, ECC_GameTraceChannel1, CollisionQueryParams)) {
if (HitResult.bBlockingHit) {
	//检测到就移动攻击
	MyPawn->MoveToEnemyAndAttackOnServer(HitResult.ImpactPoint, Cast<APawn>(HitResult.Actor));
	}
}
```



## Behaviour Tree / 行为树

### BlackBorad / 黑板

### BT - Composition Node / 复合节点

### BT - Service Node / 服务节点

### BT - Task Node / 任务节点

### AI Controller / AI 控制器
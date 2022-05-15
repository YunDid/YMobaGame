#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "../../../YMobaGameHUD.h"
#include "UI_MiniMap.generated.h"

class UCanvasPanel;
class ASceneCapture2D;
class UImage;
class UMaterialInterface;
class UCanvasPanelSlot;

UCLASS()
class UUI_MiniMap : public UUI_Base
{
	GENERATED_BODY()

	//小地图画布.
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* MiniMapPanel;

	//截图照片.
	UPROPERTY(meta = (BindWidget))
	UImage* MiniMapImage;

	//2D捕捉.
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<ASceneCapture2D> CaptureClass;

	//材质.
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TScriptInterface<UMaterialInterface> MiniMapMaterial;

public:
	//初始化.
	virtual void NativeConstruct() override;
	//逐帧.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//判断 CharacterIcons 中是否存放了对应角色的小地图图标.
	bool IsExistence(const int64 InID);
	//更新角色图标位置.
	void ResetLocation(UCanvasPanelSlot* PanelSlot, const FVector2D& MinMapPos, const FVector2D& LocalSize2D);

protected:
	//小地图角色图标.
	TMap<int64, TWeakObjectPtr<UCanvasPanelSlot>> CharacterIcons;
private:
	//小地图大小.
	FVector2D MapSize;
};

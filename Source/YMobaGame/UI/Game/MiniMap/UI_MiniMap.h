#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_MiniMap.generated.h"

class UCanvasPanel;
class ASceneCapture2D;
class UImage;
class UMaterialInterface;
class UCanvasPanelSlot;
/**
 *
 */
UCLASS()
class UUI_MiniMap : public UUI_Base
{
	GENERATED_BODY()

	//小地图画布
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* MiniMap;

	//截图
	UPROPERTY(meta = (BindWidget))
	UImage* MiniMapImage;

	//2D捕捉相机
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<ASceneCapture2D> CaptureClass;

	//材质
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TScriptInterface<UMaterialInterface> MiniMapMat;

protected:
	////初始化
	//virtual void NativeConstruct() override;
	////逐帧
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};

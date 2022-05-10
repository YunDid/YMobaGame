
#include "UI_Base.h"

AYMobaGameHUD* UUI_Base::GetHUD() {
	return Cast<AYMobaGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}
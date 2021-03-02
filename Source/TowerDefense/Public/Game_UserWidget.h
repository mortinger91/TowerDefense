// Unreal Engine 4 Tower Defense

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game_UserWidget.generated.h"

UCLASS(Abstract)
class TOWERDEFENSE_API UGame_UserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UWidgetAnimation* GetAnimationByName(FName AnimationName) const;

	//bool PlayAnimationByName(FName AnimationName, float StartAtTime, int32 NumLoopsToPlay, EUMGSequencePlayMode::Type PlayMode, float PlaybackSpeed);

protected:
	TMap<FName, UWidgetAnimation*> AnimationsMap;

	void FillAnimationsMap();
};

// Unreal Engine 4 Tower Defense

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Game_HUD.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API AGame_HUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AGame_HUD();
	
	void UpdateHealthText(FText NewHealthText);

	void UpdateGoldText(int32 NewGoldCount);

	UFUNCTION()
	void PlayGameOverAnimation();

	void ShowTowerTooltip();

	void HideTowerTooltip();

private:
	virtual void BeginPlay() override;

	virtual void DrawHUD() override;

	TSubclassOf<class UGame_UserWidget> HUDWidgetClass;

	class UGame_UserWidget * GameUIWidget;

	class ATower_GameMode* GM;

	UFUNCTION()
	void QuitGameAction();

	UFUNCTION()
	void LevelUpAction();

	UFUNCTION()
	void SellAction();

	UFUNCTION()
	void SpawnTowerCannonAction();

	UFUNCTION()
	void SpawnTowerIceAction();
};

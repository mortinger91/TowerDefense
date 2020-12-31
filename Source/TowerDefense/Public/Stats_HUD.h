// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Stats_HUD.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API AStats_HUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AStats_HUD();
	
	virtual void BeginPlay() override;

	virtual void DrawHUD() override;

	class UGame_UserWidget * GetHUDWidget();

	void UpdateGoldText(int32 NewGoldCount);

	//void UpdateHealthText(int32 NewHealth);

	void GameOverMode();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	//class UWidgetAnimation* GameOverAnimation;

private:
	//UPROPERTY(EditAnywhere, Category = "Stats")
	//TSubclassOf<class UUserWidget> HUDWidgetClass;

	//UPROPERTY(EditAnywhere, Category = "Stats")
	//class UUserWidget * CurrentWidget;

	UPROPERTY(EditAnywhere, Category = "Stats")
	TSubclassOf<class UGame_UserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Stats")
	class UGame_UserWidget * CurrentWidget;

};

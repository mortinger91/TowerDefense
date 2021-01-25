// Fill out your copyright notice in the Description page of Project Settings.


#include "Stats_HUD.h"
#include "Tower_GameMode.h"
#include "Game_UserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Tower.h"

AStats_HUD::AStats_HUD()
{
	static ConstructorHelpers::FClassFinder<UGame_UserWidget> HealthBarObj(TEXT("/Game/Tower_Defense/UI/Game_UI"));
	HUDWidgetClass = HealthBarObj.Class;
	
}

void AStats_HUD::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass != nullptr)
	{
		GameUIWidget = CreateWidget<UGame_UserWidget>(GetWorld(), HUDWidgetClass);

		if (GameUIWidget != nullptr)
		{
			GameUIWidget->AddToViewport();
		}
	}

	GM = Cast<ATower_GameMode>(GetWorld()->GetAuthGameMode());
	if (GM == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("In Stats_HUD: Game Mode not found!"))
	}

	Cast<UButton>(GameUIWidget->GetWidgetFromName(FName("QuitGame_Button")))->OnClicked.AddDynamic(this, &AStats_HUD::QuitGameAction);

	Cast<UButton>(GameUIWidget->GetWidgetFromName(FName("Tooltip_UpgradeButton")))->OnClicked.AddDynamic(this, &AStats_HUD::LevelUpAction);

	Cast<UButton>(GameUIWidget->GetWidgetFromName(FName("Button_TowerCannon")))->OnClicked.AddDynamic(this, &AStats_HUD::SpawnTowerCannonAction);
}


void AStats_HUD::DrawHUD()
{
	Super::DrawHUD();

}

// update the health text in the player hud
void AStats_HUD::UpdateHealthText(FText NewHealthText)
{
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Health_Current")))->SetText(NewHealthText);
}

// update the gold count in the player hud
void AStats_HUD::UpdateGoldText(int32 NewGoldCount)
{
	FString G = FString::FromInt(NewGoldCount);
	FText GText = FText::FromString(G);

	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Gold_Text")))->SetText(GText);
}

// game over animation, called in game mode when gameover state is set
void AStats_HUD::PlayGameOverAnimation()
{
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("GAME_OVER_Text")))->SetVisibility(ESlateVisibility::Visible);

	UWidgetAnimation* anim = GameUIWidget->GetAnimationByName(FName("Fade"));
	GameUIWidget->PlayAnimation(anim);
}

void AStats_HUD::ShowTowerTooltip()
{
	FText TowerType = FText::FromString(FString(TEXT("Type: ")) + GM->selectedTower->towerType);
	FText Level = FText::FromString(FString(TEXT("Lvl: ")) + FString::FromInt(GM->selectedTower->GetLevel()));
	FText UpgradeGold = FText::FromString(FString::FromInt(GM->selectedTower->GetGoldToUpgrade()) + FString(TEXT("g")));
										
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_Type")))->SetText(TowerType);
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_Level")))->SetText(Level);
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_UpgradeGold")))->SetText(UpgradeGold);

	Cast<UImage>(GameUIWidget->GetWidgetFromName(FName("Tooltip_Background")))->SetVisibility(ESlateVisibility::Visible);
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_Type")))->SetVisibility(ESlateVisibility::Visible); 
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_Level")))->SetVisibility(ESlateVisibility::Visible);
	Cast<UButton>(GameUIWidget->GetWidgetFromName(FName("Tooltip_UpgradeButton")))->SetVisibility(ESlateVisibility::Visible);
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_UpgradeGold")))->SetVisibility(ESlateVisibility::Visible);

	if (GM->selectedTower->GetLevel() == 3)
	{
		Cast<UButton>(GameUIWidget->GetWidgetFromName(FName("Tooltip_UpgradeButton")))->SetVisibility(ESlateVisibility::Hidden);
		Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_UpgradeGold")))->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AStats_HUD::HideTowerTooltip()
{
	Cast<UImage>(GameUIWidget->GetWidgetFromName(FName("Tooltip_Background")))->SetVisibility(ESlateVisibility::Hidden);
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_Type")))->SetVisibility(ESlateVisibility::Hidden);
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_Level")))->SetVisibility(ESlateVisibility::Hidden);
	Cast<UButton>(GameUIWidget->GetWidgetFromName(FName("Tooltip_UpgradeButton")))->SetVisibility(ESlateVisibility::Hidden);
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_UpgradeGold")))->SetVisibility(ESlateVisibility::Hidden);
}

void AStats_HUD::QuitGameAction()
{
	GM->ChangeGamePlayState(EGamePlayState::EGameOver);
}

void AStats_HUD::LevelUpAction()
{
	GM->LevelUpSelectedTower();
}

void AStats_HUD::SpawnTowerCannonAction()
{
	GM->SpawnTower("Cannon");
}

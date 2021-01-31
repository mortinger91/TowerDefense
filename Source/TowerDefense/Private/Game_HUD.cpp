// Unreal Engine 4 Tower Defense


#include "Game_HUD.h"
#include "Tower_GameMode.h"
#include "Game_UserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Tower.h"

AGame_HUD::AGame_HUD()
{
	static ConstructorHelpers::FClassFinder<UGame_UserWidget> HealthBarObj(TEXT("/Game/Tower_Defense/UI/Game_UI"));
	HUDWidgetClass = HealthBarObj.Class;
}

void AGame_HUD::BeginPlay()
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
		UE_LOG(LogActor, Warning, TEXT("In Game_HUD: Game Mode not found!"))
	}

	Cast<UButton>(GameUIWidget->GetWidgetFromName(FName("QuitGame_Button")))->OnClicked.AddDynamic(this, &AGame_HUD::QuitGameAction);

	Cast<UButton>(GameUIWidget->GetWidgetFromName(FName("Tooltip_UpgradeButton")))->OnClicked.AddDynamic(this, &AGame_HUD::LevelUpAction);

	Cast<UButton>(GameUIWidget->GetWidgetFromName(FName("Tooltip_SellButton")))->OnClicked.AddDynamic(this, &AGame_HUD::SellAction);

	Cast<UButton>(GameUIWidget->GetWidgetFromName(FName("Button_TowerCannon")))->OnClicked.AddDynamic(this, &AGame_HUD::SpawnTowerCannonAction);

}

void AGame_HUD::DrawHUD()
{
	Super::DrawHUD();

}

// update the health text in the player hud
void AGame_HUD::UpdateHealthText(FText NewHealthText)
{
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Health_Current")))->SetText(NewHealthText);
}

// update the gold count in the player hud
void AGame_HUD::UpdateGoldText(int32 NewGoldCount)
{
	FString G = FString::FromInt(NewGoldCount);
	FText GText = FText::FromString(G);

	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Gold_Text")))->SetText(GText);
}

// game over animation, called in game mode when gameover state is set
void AGame_HUD::PlayGameOverAnimation()
{
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("GAME_OVER_Text")))->SetVisibility(ESlateVisibility::Visible);

	UWidgetAnimation* anim = GameUIWidget->GetAnimationByName(FName("Fade"));
	GameUIWidget->PlayAnimation(anim);
}

void AGame_HUD::ShowTowerTooltip()
{
	FText TowerType = FText::FromString(FString(TEXT("Type: ")) + GM->selectedTower->GetTowerType());
	FText Level = FText::FromString(FString(TEXT("Lvl: ")) + FString::FromInt(GM->selectedTower->GetLevel()));
	FText UpgradeGold = FText::FromString(FString::FromInt(GM->selectedTower->GetGoldToUpgrade()) + FString(TEXT("g")));
	FText SellGold = FText::FromString(FString::FromInt(GM->selectedTower->GetGoldToSell()) + FString(TEXT("g")));
										
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_Type")))->SetText(TowerType);
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_Level")))->SetText(Level);
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_UpgradeGold")))->SetText(UpgradeGold);
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_SellGold")))->SetText(SellGold);

	Cast<UImage>(GameUIWidget->GetWidgetFromName(FName("Tooltip_Background")))->SetVisibility(ESlateVisibility::Visible);
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_Type")))->SetVisibility(ESlateVisibility::Visible); 
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_Level")))->SetVisibility(ESlateVisibility::Visible);
	Cast<UButton>(GameUIWidget->GetWidgetFromName(FName("Tooltip_UpgradeButton")))->SetVisibility(ESlateVisibility::Visible);
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_UpgradeGold")))->SetVisibility(ESlateVisibility::Visible);
	Cast<UButton>(GameUIWidget->GetWidgetFromName(FName("Tooltip_SellButton")))->SetVisibility(ESlateVisibility::Visible);
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_SellGold")))->SetVisibility(ESlateVisibility::Visible);

	if (GM->selectedTower->GetLevel() == 3)
	{
		Cast<UButton>(GameUIWidget->GetWidgetFromName(FName("Tooltip_UpgradeButton")))->SetVisibility(ESlateVisibility::Hidden);
		Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_UpgradeGold")))->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AGame_HUD::HideTowerTooltip()
{
	Cast<UImage>(GameUIWidget->GetWidgetFromName(FName("Tooltip_Background")))->SetVisibility(ESlateVisibility::Hidden);
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_Type")))->SetVisibility(ESlateVisibility::Hidden);
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_Level")))->SetVisibility(ESlateVisibility::Hidden);
	Cast<UButton>(GameUIWidget->GetWidgetFromName(FName("Tooltip_UpgradeButton")))->SetVisibility(ESlateVisibility::Hidden);
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_UpgradeGold")))->SetVisibility(ESlateVisibility::Hidden);
	Cast<UButton>(GameUIWidget->GetWidgetFromName(FName("Tooltip_SellButton")))->SetVisibility(ESlateVisibility::Hidden);
	Cast<UTextBlock>(GameUIWidget->GetWidgetFromName(FName("Tooltip_SellGold")))->SetVisibility(ESlateVisibility::Hidden);
}

void AGame_HUD::QuitGameAction()
{
	GM->ChangeGamePlayState(EGamePlayState::EGameOver);
}

void AGame_HUD::LevelUpAction()
{
	GM->LevelUpSelectedTower();
}

void AGame_HUD::SellAction()
{
	GM->SellSelectedTower();
}

void AGame_HUD::SpawnTowerCannonAction()
{
	GM->SpawnTower("Cannon");
}

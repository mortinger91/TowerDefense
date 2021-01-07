// Fill out your copyright notice in the Description page of Project Settings.


#include "Stats_HUD.h"
#include "Tower_GameMode.h"
#include "Game_UserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/Button.h"

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
		CurrentWidget = CreateWidget<UGame_UserWidget>(GetWorld(), HUDWidgetClass);

		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}

	GM = Cast<ATower_GameMode>(GetWorld()->GetAuthGameMode());
	if (GM == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("In Stats_HUD: Game Mode not found!"))
	}

	Cast<UButton>(CurrentWidget->GetWidgetFromName(FName("QuitGame_Button")))->OnClicked.AddDynamic(this, &AStats_HUD::QuitGameAction);
}


void AStats_HUD::DrawHUD()
{
	Super::DrawHUD();

}

// update the health text in the player hud
void AStats_HUD::UpdateHealthText(FText NewHealthText)
{
	Cast<UTextBlock>(GetHUDWidget()->GetWidgetFromName(FName("Health_Text")))->SetText(NewHealthText);
}

// update the gold count in the player hud
void AStats_HUD::UpdateGoldText(int32 NewGoldCount)
{
	FString G = FString::FromInt(NewGoldCount);
	FText GText = FText::FromString(G);

	Cast<UTextBlock>(GetHUDWidget()->GetWidgetFromName(FName("Gold_Text")))->SetText(GText);
}

// game over animation, called in game mode when gameover state is set
void AStats_HUD::GameOverMode()
{
	Cast<UTextBlock>(GetHUDWidget()->GetWidgetFromName(FName("GAME_OVER_Text")))->SetVisibility(ESlateVisibility::Visible);

	UWidgetAnimation* anim = GetHUDWidget()->GetAnimationByName(FName("Fade"));
	GetHUDWidget()->PlayAnimation(anim);
}

UGame_UserWidget * AStats_HUD::GetHUDWidget()
{
	return CurrentWidget;
}

void AStats_HUD::QuitGameAction()
{
	GM->ChangeGamePlayState(EGamePlayState::EGameOver);
}
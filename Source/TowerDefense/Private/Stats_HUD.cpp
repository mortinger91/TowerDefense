// Fill out your copyright notice in the Description page of Project Settings.


#include "Stats_HUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

AStats_HUD::AStats_HUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarObj(TEXT("/Game/Tower_Defense/UI/Health_UI"));
	HUDWidgetClass = HealthBarObj.Class;
	
}

void AStats_HUD::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void AStats_HUD::DrawHUD()
{
	Super::DrawHUD();

}

// update the gold count in the player hud
void AStats_HUD::UpdateGoldText(int32 NewGoldCount)
{
	FString G = FString::FromInt(NewGoldCount);
	FText GText = FText::FromString(G);

	Cast<UTextBlock>(GetHUDWidget()->GetWidgetFromName(FName("Gold_Text")))->SetText(GText);
}

UUserWidget * AStats_HUD::GetHUDWidget()
{
	return CurrentWidget;
}

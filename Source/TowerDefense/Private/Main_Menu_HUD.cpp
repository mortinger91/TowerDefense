// #pragma optimize("", off)


#include "Main_Menu_HUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

AMain_Menu_HUD::AMain_Menu_HUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarObj(TEXT("/Game/Tower_Defense/UI/Main_Menu_UI"));
	HUDWidgetClass = HealthBarObj.Class;
	
}

void AMain_Menu_HUD::BeginPlay()
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

void AMain_Menu_HUD::DrawHUD()
{
	Super::DrawHUD();

}

UUserWidget * AMain_Menu_HUD::GetHUDWidget()
{
	return CurrentWidget;
}


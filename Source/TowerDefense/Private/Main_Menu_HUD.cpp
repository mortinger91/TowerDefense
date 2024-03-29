// Unreal Engine 4 Tower Defense
// #pragma optimize("", off)

#include "Main_Menu_HUD.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/GameUserSettings.h"

//TODO: fix DPI scaling, only solution atm is to package and disable DPI optimization performed by application. Also packaged windows starts in fullscreen

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

	Cast<UButton>(CurrentWidget->GetWidgetFromName(FName("StartGame")))->OnClicked.AddDynamic(this, &AMain_Menu_HUD::StartGame);
	Cast<UButton>(CurrentWidget->GetWidgetFromName(FName("Options")))->OnClicked.AddDynamic(this, &AMain_Menu_HUD::OpenOptions);
	Cast<UButton>(CurrentWidget->GetWidgetFromName(FName("QuitGame")))->OnClicked.AddDynamic(this, &AMain_Menu_HUD::QuitGame);

	Cast<UButton>(CurrentWidget->GetWidgetFromName(FName("1080")))->OnClicked.AddDynamic(this, &AMain_Menu_HUD::SetRes1080);
	Cast<UButton>(CurrentWidget->GetWidgetFromName(FName("720")))->OnClicked.AddDynamic(this, &AMain_Menu_HUD::SetRes720);
	Cast<UButton>(CurrentWidget->GetWidgetFromName(FName("480")))->OnClicked.AddDynamic(this, &AMain_Menu_HUD::SetRes480);
	Cast<UButton>(CurrentWidget->GetWidgetFromName(FName("ReturnToMenu")))->OnClicked.AddDynamic(this, &AMain_Menu_HUD::ReturnToMainMenu);

}

void AMain_Menu_HUD::DrawHUD()
{
	Super::DrawHUD();

}

UUserWidget * AMain_Menu_HUD::GetHUDWidget() const
{
	return CurrentWidget;
}

void AMain_Menu_HUD::StartGame()
{
	UE_LOG(LogActor, Warning, TEXT("Game Started!"))
	//UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	UGameplayStatics::OpenLevel(this, FName(TEXT("/Game/Tower_Defense/Levels/Level1")));
}

void AMain_Menu_HUD::QuitGame()
{
	UE_LOG(LogActor, Warning, TEXT("Quitting Game!"))
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}

void AMain_Menu_HUD::OpenOptions()
{
	Cast<UVerticalBox>(CurrentWidget->GetWidgetFromName(FName("Main_Menu")))->SetVisibility(ESlateVisibility::Hidden);
	Cast<UVerticalBox>(CurrentWidget->GetWidgetFromName(FName("Options_Menu")))->SetVisibility(ESlateVisibility::Visible);
}

void AMain_Menu_HUD::SetRes1080()
{
	//GEngine->GameUserSettings->SetScreenResolution(FIntPoint(1920,1080));
	UE_LOG(LogActor, Warning, TEXT("Console executing: setres 1920x1080"))
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("setres 1920x1080");
}

void AMain_Menu_HUD::SetRes720()
{
	//GEngine->GameUserSettings->SetScreenResolution(FIntPoint(1280,720));
	UE_LOG(LogActor, Warning, TEXT("Console executing: setres 1280x720"))
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("setres 1280x720");
}

void AMain_Menu_HUD::SetRes480()
{
	//GEngine->GameUserSettings->SetScreenResolution(FIntPoint(640,480));
	UE_LOG(LogActor, Warning, TEXT("Console executing: setres 640x480"))
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("setres 640x480");
}

void AMain_Menu_HUD::ReturnToMainMenu()
{
	Cast<UVerticalBox>(CurrentWidget->GetWidgetFromName(FName("Main_Menu")))->SetVisibility(ESlateVisibility::Visible); 
	Cast<UVerticalBox>(CurrentWidget->GetWidgetFromName(FName("Options_Menu")))->SetVisibility(ESlateVisibility::Hidden);
}


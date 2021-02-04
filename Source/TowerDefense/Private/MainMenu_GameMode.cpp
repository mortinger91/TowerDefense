// Unreal Engine 4 Tower Defense
// #pragma optimize("", off)

#include "MainMenu_GameMode.h"
#include "Engine/World.h"

AMainMenu_GameMode::AMainMenu_GameMode()
{

}

void AMainMenu_GameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogActor, Warning, TEXT("Loaded MainMenu_GameMode"))
	
	PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		FInputModeGameAndUI data;
		data.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		PC->SetInputMode(data);
		PC->bShowMouseCursor = true; 
		PC->bEnableClickEvents = true; 
		PC->bEnableMouseOverEvents = true;
	}
}

// Called every frame
void AMainMenu_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
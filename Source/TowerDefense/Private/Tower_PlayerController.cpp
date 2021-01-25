// Unreal Engine 4 Tower Defense
#pragma optimize("", off)

#include "Tower_PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "CustomPawn.h"
#include "Tower.h"
#include "Tower_GameMode.h"
#include "Stats_HUD.h"
#include "DrawDebugHelpers.h"

//#include "GenericPlatform/ICursor.h"
//#include "Blueprint/UserWidget.h"
//#include "UObject/ConstructorHelpers.h"

ATower_PlayerController::ATower_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	//GetHitResultUnderCursorByChannel
	
	//GetHitResultUnderCursorByChannel
	//PlayerInput->

	//static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarObj(TEXT("/Game/Tower_Defense/UI/DefaultCursor"));
	//CursorWidgetClass = Cast<UUserWidget>(HealthBarObj.Class);
}

void ATower_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI data;
	data.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	SetInputMode(data);
	bShowMouseCursor = true; 
	bEnableClickEvents = true; 
	bEnableMouseOverEvents = true;
	
	CurrentMouseCursor = EMouseCursor::Crosshairs;
	//SetMouseCursorWidget(GetMouseCursor(), CursorWidgetClass);

	GM = Cast<ATower_GameMode>(GetWorld()->GetAuthGameMode());
	if (GM == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("In Tower_PlayerController: Game Mode not found!"))
	}

}

void ATower_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATower_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &ATower_PlayerController::ClickAction);
	InputComponent->BindAction("LeftMouseClick", IE_Released, this, &ATower_PlayerController::ReleaseAction);

	InputComponent->BindAxis("MoveForward", this, &ATower_PlayerController::MoveForward); 
	InputComponent->BindAxis("MoveRight", this, &ATower_PlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &ATower_PlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &ATower_PlayerController::LookUp);
}

void ATower_PlayerController::MoveForward(float Amount)
{
	Cast<ACustomPawn>(GetPawn())->MoveForward(Amount);
}

void ATower_PlayerController::MoveRight(float Amount)
{
	Cast<ACustomPawn>(GetPawn())->MoveRight(Amount);
}

void ATower_PlayerController::Turn(float Amount)
{
	Cast<ACustomPawn>(GetPawn())->Turn(Amount);
}

void ATower_PlayerController::LookUp(float Amount)
{
	Cast<ACustomPawn>(GetPawn())->LookUp(Amount);
}

void ATower_PlayerController::ClickAction()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Click Pressed")));

	// DEBUG DRAW LINE ON CLICK
		//FVector mouseLocation;
		//FVector mouseDirection;
		//GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(mouseLocation, mouseDirection);
		//FHitResult OutHit;
		//FVector Start = mouseLocation;
		//FVector End = mouseLocation + mouseDirection*10000;
		//DrawDebugLine(GetWorld(), Start, End, FColor::Green, true, 2.f, false, 4.f);

	if (GM->dragMode)
	{
		GM->FinalizeTowerSpawn();
	}
	else
	{
		FHitResult Result;
		GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, Result);
		if(ATower * tower = Cast<ATower>(Result.Actor))
		{
			GM->selectedTower = tower;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Clicked Tower Actor: %s"), *GM->selectedTower->GetName()));

			GM->HudWidgetPlayer->ShowTowerTooltip();
		} else
		{
			GM->selectedTower = nullptr;
			GM->HudWidgetPlayer->HideTowerTooltip();
		}
	}
}

void ATower_PlayerController::ReleaseAction()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Click Released")));
	//if (GM->dragMode)
	//{
	//	GM->FinalizeTowerSpawn();
	//}
}

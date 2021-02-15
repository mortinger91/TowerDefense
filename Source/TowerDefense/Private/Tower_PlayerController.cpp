// Unreal Engine 4 Tower Defense
// #pragma optimize("", off)

#include "Tower_PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "CustomPawn.h"
#include "Tower.h"
#include "Tower_GameMode.h"
#include "Game_HUD.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
//#include "DrawDebugHelpers.h"
#include "TowerBase.h"

ATower_PlayerController::ATower_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	leftMouseIsClicked = false;
	//rightMouseIsClicked = false;
}

void ATower_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	ConsoleCommand(FString("t.MaxFPS = 60"), false);

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

	// DEBUG MOUSE POSITION
		FVector mouseLocationD;
		FVector mouseDirectionD;
		DeprojectMousePositionToWorld(mouseLocationD, mouseDirectionD);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("mouse position: x: %f, y: %f, z: %f"), mouseLocationD.X, mouseLocationD.Y, mouseLocationD.Z ));

	if (GM->dragMode)
	{
		FVector mouseLocation;
		FVector mouseDirection;
		DeprojectMousePositionToWorld(mouseLocation, mouseDirection);
		
		FHitResult OutHit;
		FVector Start = mouseLocation;
		FVector End = mouseLocation + mouseDirection*10000;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GM->spawnedTower);
		GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_WorldStatic, CollisionParams);
		if (OutHit.GetActor() != nullptr && OutHit.GetActor()->IsA(ATowerBase::StaticClass()))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Tower onto base: %s"), *OutHit.GetActor()->GetName()));
			GM->spawnedTower->SetActorLocation(OutHit.GetActor()->GetActorLocation());
			GM->selectedBase = Cast<ATowerBase>(OutHit.GetActor());
		}
		else
		{
			GM->spawnedTower->SetActorLocation(OutHit.Location);
			GM->selectedBase = nullptr;
		}
	}
}

void ATower_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &ATower_PlayerController::ClickAction);
	InputComponent->BindAction("LeftMouseClick", IE_Released, this, &ATower_PlayerController::ReleaseAction);
	//InputComponent->BindAction("RightMouseClick", IE_Pressed, this, &ATower_PlayerController::RightClickAction);
	//InputComponent->BindAction("RightMouseClick", IE_Released, this, &ATower_PlayerController::RightReleaseAction);
	InputComponent->BindAction("Touch1", IE_Pressed, this, &ATower_PlayerController::TouchClickAction);
	InputComponent->BindAction("Touch1", IE_Released, this, &ATower_PlayerController::TouchReleaseAction);

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
	//if (leftMouseIsClicked && !GM->dragMode)
	if (!GM->dragMode)
	{
		Cast<ACustomPawn>(GetPawn())->Turn(Amount/2);
	}
}

void ATower_PlayerController::LookUp(float Amount)
{
	//if (leftMouseIsClicked && !GM->dragMode)
	if (!GM->dragMode)
	{
		Cast<ACustomPawn>(GetPawn())->LookUp(Amount/2);
	}
}

void ATower_PlayerController::ClickAction()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Mouse click action")));

	// DEBUG DRAW LINE ON CLICK
		//FVector mouseLocation;
		//FVector mouseDirection;
		//DeprojectMousePositionToWorld(mouseLocation, mouseDirection);
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("mouse position: x: %f, y: %f, z: %f"), mouseLocation.X, mouseLocation.Y, mouseLocation.Z ));
		//FHitResult OutHit;
		//FVector Start = mouseLocation;
		//FVector End = mouseLocation + mouseDirection*10000;
		//DrawDebugLine(GetWorld(), Start, End, FColor::Green, true, 2.f, false, 4.f);

	if (!GM->dragMode)
	{
		FHitResult Result;
		GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, Result);
		if(ATower * tower = Cast<ATower>(Result.Actor))
		{
			GM->selectedTower = tower;
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Clicked Tower Actor: %s"), *GM->selectedTower->GetName()));
			GM->HudWidgetPlayer->ShowTowerTooltip();
		} else
		{
			GM->selectedTower = nullptr;
			GM->HudWidgetPlayer->HideTowerTooltip();
			leftMouseIsClicked = true;
		}
	}
}

void ATower_PlayerController::ReleaseAction()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Mouse Released click action")));
	leftMouseIsClicked = false;
	if (GM->dragMode)
	{
		GM->FinalizeTowerSpawn();
	}
}

//void ATower_PlayerController::RightClickAction()
//{
//	//rightMouseIsClicked = true;
//}
//
//void ATower_PlayerController::RightReleaseAction()
//{
//	//rightMouseIsClicked = false;
//}

void ATower_PlayerController::TouchClickAction()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Touch click action")));
	//ClickAction();
	FHitResult Result;
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, Result);
	if(ATower * tower = Cast<ATower>(Result.Actor))
	{
		GM->selectedTower = tower;
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Clicked Tower Actor: %s"), *GM->selectedTower->GetName()));
		GM->HudWidgetPlayer->ShowTowerTooltip();
	} else
	{
		GM->selectedTower = nullptr;
		GM->HudWidgetPlayer->HideTowerTooltip();
		//leftMouseIsClicked = true;
	}
}

void ATower_PlayerController::TouchReleaseAction()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Touch Released click action")));
	ReleaseAction();
}

// Unreal Engine 4 Tower Defense
 #pragma optimize("", off)

#include "Tower_PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "CustomPawn.h"
#include "Tower.h"
#include "Tower_GameMode.h"
#include "Game_HUD.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "TowerBase.h"
#include "GenericPlatform/GenericPlatformMath.h"
//#include "Enemy.h"

ATower_PlayerController::ATower_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	leftMouseIsClicked = false;
	previousDistance = 0.f;
	newDistance = 0.f;
}

void ATower_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// engine custom settings
	ConsoleCommand(FString("t.MaxFPS = 60"), false);
	ConsoleCommand(FString("r.Shadow.RadiusThreshold 0.01"), false);

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
		//FVector mouseLocationD;
		//FVector mouseDirectionD;
		//DeprojectMousePositionToWorld(mouseLocationD, mouseDirectionD);
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("mouse position: x: %f, y: %f, z: %f"), mouseLocationD.X, mouseLocationD.Y, mouseLocationD.Z ));

	// spawning tower on desktop and mobile
	if (GM->dragMode)
	{
		FVector mouseLocation;
		FVector mouseDirection;
		DeprojectMousePositionToWorld(mouseLocation, mouseDirection);

		FHitResult OutHit;
		FVector Start = mouseLocation;
		FVector End = mouseLocation + mouseDirection*10000;
		if (GM->mobile)
		{
			End += FVector(0.f, -1200.f, 0.f);
		}

		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("mouse position: x: %f, y: %f, z: %f"), End.X, End.Y, End.Z ));

		//FCollisionQueryParams CollisionParams;
		//CollisionParams.AddIgnoredActor(GM->spawnedTower);
		GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_WorldStatic);//, CollisionParams);

		//if (OutHit.GetActor()->IsA(ATowerBase::StaticClass()) && !Cast<ATowerBase>(OutHit.GetActor())->used)
		bool toMove = true;
		//if (OutHit.GetActor() != nullptr && OutHit.GetActor()->IsA(ATowerBase::StaticClass()))
		if (ATowerBase* base = Cast<ATowerBase>(OutHit.GetActor()))
		{
			if (!base->used)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Tower onto base: %s"), *OutHit.GetActor()->GetName()));
				//GM->spawnedTower->SetActorLocation(OutHit.GetActor()->GetActorLocation());
				GM->spawnedTower->SetActorLocation(base->GetActorLocation());
				//GM->selectedBase = Cast<ATowerBase>(OutHit.GetActor()); 
				GM->selectedBase = base;
				toMove = false;
			}
		}
		//else if ( OutHit.GetActor()->IsA(AEnemy::StaticClass()) )
		//{
		//	toMove = false;
		//}
		if (toMove)
		{
			GM->spawnedTower->SetActorLocation(OutHit.Location);
			GM->selectedBase = nullptr;
		}
	}
	// for mobile movement
	else if (GM->mobile)
	{
		bool isCurrentlyPressed1;
		GetInputTouchState(ETouchIndex::Touch1, newTouchLocation1.X, newTouchLocation1.Y, isCurrentlyPressed1);
		if (isCurrentlyPressed1)
		{
			bool isCurrentlyPressed2;
			GetInputTouchState(ETouchIndex::Touch2, touchLocation2.X, touchLocation2.Y, isCurrentlyPressed2);
			if (isCurrentlyPressed2)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("touch2 is pressed")));

				newDistance = DistanceOfTwoPoints(newTouchLocation1, touchLocation2);
				float zoomAmount = previousDistance - newDistance;
				FVector deltaOffsetZoom(0.f, 0.f, zoomAmount * 2);
				Cast<ACustomPawn>(GetPawn())->AddActorWorldOffset(deltaOffsetZoom);

				previousDistance = newDistance;
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("touch1 is pressed")));

				FVector2D deltaTouchLocation = newTouchLocation1 - previousTouchLocation1;
				FVector deltaOffsetMove(deltaTouchLocation.Y * 2, -deltaTouchLocation.X * 2, 0.f);
				Cast<ACustomPawn>(GetPawn())->AddActorWorldOffset(deltaOffsetMove);
			}

			previousTouchLocation1 = newTouchLocation1;
		}
	}
}

void ATower_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &ATower_PlayerController::ClickAction);
	InputComponent->BindAction("LeftMouseClick", IE_Released, this, &ATower_PlayerController::ReleaseAction);

	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ATower_PlayerController::OnTouchBegin);
	InputComponent->BindTouch(EInputEvent::IE_Released, this, &ATower_PlayerController::TouchReleaseAction);

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
	if (leftMouseIsClicked && !GM->dragMode)
	//if (!GM->dragMode)
	{
		Cast<ACustomPawn>(GetPawn())->Turn(Amount/2);
	}
}

void ATower_PlayerController::LookUp(float Amount)
{
	if (leftMouseIsClicked && !GM->dragMode)
	//if (!GM->dragMode)
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
		//mouseDirection *= 10000;
		//FVector End = mouseLocation + mouseDirection;
		//End.Y += End.Y/5;
		//DrawDebugLine(GetWorld(), Start, End, FColor::Green, true, 2.f, false, 4.f);

	if (!GM->dragMode)
	{
		FHitResult Result;
		GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, Result);
		if(ATower * tower = Cast<ATower>(Result.GetActor()))
		{
			GM->selectedTower = tower;
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Clicked Tower Actor: %s"), *GM->selectedTower->GetName()));
			GM->HudWidgetPlayer->ShowTowerTooltip();
		} 
		else
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

void ATower_PlayerController::OnTouchBegin(ETouchIndex::Type touchType, FVector touchLocation)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Touch click action")));

	bool isCurrentlyPressed;
	if (touchType == ETouchIndex::Touch1)
	{
		GetInputTouchState(ETouchIndex::Touch1, previousTouchLocation1.X, previousTouchLocation1.Y, isCurrentlyPressed);
		ClickAction();
	}
	if (touchType == ETouchIndex::Touch2)
	{
		GetInputTouchState(ETouchIndex::Touch2, touchLocation2.X, touchLocation2.Y, isCurrentlyPressed);
		previousDistance = DistanceOfTwoPoints(previousTouchLocation1, touchLocation2);
	}
}

void ATower_PlayerController::TouchReleaseAction(ETouchIndex::Type touchType, FVector touchLocation)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Touch Released click action")));

	if (touchType == ETouchIndex::Touch1)
	{
		ReleaseAction();
	}
}

inline float ATower_PlayerController::DistanceOfTwoPoints(const FVector2D& p1, const FVector2D& p2) const
{
	return FGenericPlatformMath::Sqrt( FGenericPlatformMath::Pow((p1.X - p2.X),2.f) + FGenericPlatformMath::Pow((p1.Y - p2.Y),2.f) );
}
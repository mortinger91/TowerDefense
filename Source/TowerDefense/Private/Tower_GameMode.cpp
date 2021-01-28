// Fill out your copyright notice in the Description page of Project Settings.
#pragma optimize("", off)

#include "Tower_GameMode.h"
#include "Tower_GameState.h"
#include "Stats_HUD.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tower.h"
#include "GameplayStats.h"
#include "Engine/Engine.h"

ATower_GameMode::ATower_GameMode() : Super()
{
	PrimaryActorTick.bCanEverTick = true;

	selectedTower = nullptr;
	spawnedTower = nullptr;

	MaxHealth = 10;
	HealthDamage = 0.0f;
	MaxGold = 999;

	InitiateGameOver = false;
	dragMode = false;

	MyTimeline = NewObject<UTimelineComponent>(this, FName("Health_Animation"));
}

void ATower_GameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogActor, Warning, TEXT("Loaded Tower_GameMode"))

	ChangeGamePlayState(EGamePlayState::EPlaying);

	GS = Cast<ATower_GameState>(GetWorld()->GetGameState());
	if (GS == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("Game State not found!"))
	}
	if (HealthCurve == nullptr)
    {
		UE_LOG(LogActor, Warning, TEXT("HealthCurve not found!"))
	}

	HudWidgetPlayer = Cast<AStats_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (HudWidgetPlayer == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("HUD Widget not found!"))
	}

	// setting health at the start of the game
	GS->Health = MaxHealth;
	PreviousHealth = GS->HealthPercentage;

	// timeline and callback
    FOnTimelineFloat TimelineCallback;
    TimelineCallback.BindUFunction(this, FName("SetHealth"));
    MyTimeline->AddInterpFloat(HealthCurve, TimelineCallback);
	MyTimeline->RegisterComponent();

	// errore all'avvio
	//HudWidgetPlayer->UpdateGoldText(GS->Gold);
}

// Called every frame
void ATower_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MyTimeline != nullptr) MyTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);

	if (dragMode)
	{
		FVector mouseLocation;
		FVector mouseDirection;
		GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(mouseLocation, mouseDirection);
		
		FHitResult OutHit;
		FVector Start = mouseLocation;
		FVector End = mouseLocation + mouseDirection*10000;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(spawnedTower);
		GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_WorldStatic, CollisionParams);
		spawnedTower->SetActorLocation(OutHit.Location);
	}
}

void ATower_GameMode::Remove1Health()
{
	if(MyTimeline != nullptr) MyTimeline->Stop();
	PreviousHealth = GS->HealthPercentage;
	HealthDamage = (-1.f/MaxHealth);
	if(MyTimeline != nullptr) MyTimeline->PlayFromStart();
}

// called when MyTimeline->PlayFromStart() is called in Remove1Health()
void ATower_GameMode::SetHealth()
{
	if (!InitiateGameOver)
	{
		TimelineValue = MyTimeline->GetPlaybackPosition();
		CurveFloatValue = PreviousHealth + HealthDamage*HealthCurve->GetFloatValue(TimelineValue);
		GS->Health = UKismetMathLibrary::Round(CurveFloatValue*MaxHealth);

		HudWidgetPlayer->UpdateHealthText(GS->GetHealthText());

		GS->HealthPercentage = CurveFloatValue;
		// if health reaches 0, enter gameover state
		if (GS->Health <= 0)
		{
			MyTimeline->Stop();
			ChangeGamePlayState(EGamePlayState::EGameOver);
		}
	}
}

// add to the player golds, return updated amount
void ATower_GameMode::UpdateGold(float gold)
{
	if (GS->Gold < MaxGold)
	{
		if ((GS->Gold + gold) < MaxGold)
		{
			GS->Gold += gold;
		}
		else
		{
			GS->Gold = MaxGold;
		}
	}
	HudWidgetPlayer->UpdateGoldText(GS->Gold);
}

bool ATower_GameMode::GoldAvailable(int32 GoldToCheck)
{
	if (GS->GetGold() >= GoldToCheck)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ATower_GameMode::ChangeGamePlayState(EGamePlayState NewState)
{
	CurrentState = NewState;
	switch (CurrentState)
	{
		case EGamePlayState::EPlaying:
		{
			// do nothing
		}
		break;
		// Unknown/default state
		case EGamePlayState::EGameOver:
		{
			InitiateGameOver = true;
			// restart the level
			UE_LOG(LogActor, Warning, TEXT("WE FUCKING LOST!"))

			HudWidgetPlayer->PlayGameOverAnimation();

			FTimerDelegate TimerDel;
			FTimerHandle TimerHandle;
 
			//Binding the function to the timeline
			TimerDel.BindUFunction(this, FName("GoToMainMenu"));
			//Calling GoToMainMenu after 3 seconds without looping
			GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 3.f, false);
		}
		break;
		// Unknown/default state
		default:
		case EGamePlayState::EUnknown:
		{
			// do nothing
		}
		break;
	}
}

void ATower_GameMode::LevelUpSelectedTower()
{
	selectedTower->LevelUp();
	HudWidgetPlayer->ShowTowerTooltip();
}

void ATower_GameMode::SellSelectedTower()
{
	selectedTower->Sell();
	HudWidgetPlayer->HideTowerTooltip();
}

void ATower_GameMode::SpawnTower(FString towerType)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.bNoFail = true;
	//SpawnParams.Owner = this;

	FTransform SpawnTransform;
	//SpawnTransform.SetLocation(FVector(200.f,600.f,200.f));
	SpawnTransform.SetLocation(FVector(0.f,0.f,0.f));

	if (towerType == "Cannon")
	{
		if (GoldAvailable(Cannon::goldToBuild))
		{
			spawnedTower = GetWorld()->SpawnActor<ATower>(CannonTower, SpawnTransform, SpawnParams);
			dragMode = true;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Not enough gold to spawn Cannon tower")));
		}
	}
	else if (towerType == "Else")
	{
		// ...
	}
}

void ATower_GameMode::FinalizeTowerSpawn()
{
	dragMode = false;
	// check tower position
	// if not ok, destroy tower
	UpdateGold(-spawnedTower->GetGoldToBuild());
	spawnedTower->Activate();
	spawnedTower = nullptr;
}

void ATower_GameMode::GoToMainMenu()
{
	UGameplayStatics::OpenLevel(this, FName(TEXT("/Game/Tower_Defense/Levels/StartMenuLevel")));
}

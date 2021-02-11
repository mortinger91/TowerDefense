// Unreal Engine 4 Tower Defense
 #pragma optimize("", off)

#include "Tower_GameMode.h"
#include "Tower_GameState.h"
#include "Game_HUD.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tower.h"
#include "GameplayStats.h"
#include "Engine/Engine.h"
#include "CannonTower.h"
#include "TowerBase.h"
#include "IceTower.h"
#include "SpawnPoint.h"

ATower_GameMode::ATower_GameMode() : Super()
{
	PrimaryActorTick.bCanEverTick = true;

	// STATS
	MaxHealth = Game::maxHealth;
	MaxGold = Game::maxGold;

	HealthDamage = 0.0f;
	InitiateGameOver = false;
	spawnedTower = nullptr;
	dragMode = false;
	selectedTower = nullptr;
	MyTimeline = NewObject<UTimelineComponent>(this, FName("Health_Animation"));

	healthMultiplier = 0.5f;
	waveCount = 0;
	normalSpeed = true;
}

void ATower_GameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogActor, Warning, TEXT("Loaded Tower_GameMode"))

	ChangeGamePlayState(EGamePlayState::EPlaying);

	GS = Cast<ATower_GameState>(GetWorld()->GetGameState());
	if (GS == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("In Tower_GameMode: Game State not found!"))
	}

	if (HealthCurve == nullptr)
    {
		UE_LOG(LogActor, Warning, TEXT("In Tower_GameMode: HealthCurve not found!"))
	}

	HudWidgetPlayer = Cast<AGame_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (HudWidgetPlayer == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("In Tower_GameMode: HUD Widget not found!"))
	}

	// setting health at the start of the game
	GS->Health = MaxHealth;
	PreviousHealth = GS->HealthPercentage;

	// timeline and callback
    FOnTimelineFloat TimelineCallback;
    TimelineCallback.BindUFunction(this, FName("SetHealth"));
    MyTimeline->AddInterpFloat(HealthCurve, TimelineCallback);
	MyTimeline->RegisterComponent();

	ShowUnusedTowerBases(true);

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), FoundActors);
	if (FoundActors.Num() == 1)
	{
		spawnPoint = Cast<ASpawnPoint>(FoundActors[0]);
	}
	else
	{
		UE_LOG(LogActor, Warning, TEXT("In Tower_GameMode: SpawnPoint not found!"))
	}
	spawnPoint->StopSpawning(5.f);
}

// Called every frame
void ATower_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MyTimeline != nullptr) MyTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);

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

void ATower_GameMode::PauseGame()
{
	GS->isPaused = !GS->isPaused;
	GetWorld()->GetFirstPlayerController()->SetPause(GS->isPaused);
}

void ATower_GameMode::ToggleForward()
{
	if (normalSpeed)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2.f);
	}
	else
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	}
	normalSpeed = !normalSpeed;
}

void ATower_GameMode::LevelUpSelectedTower()
{
	selectedTower->LevelUp();
	HudWidgetPlayer->ShowTowerTooltip();
}

void ATower_GameMode::SellSelectedTower()
{
	selectedTower->Sell();
	selectedTower->GetTowerBase()->used = false;
	HudWidgetPlayer->HideTowerTooltip();
}

// called from Game_HUD "SpawnTower***Action" when clicking tower button on Game_UI
void ATower_GameMode::SpawnTower(FString towerType)
{
	if (!GS->isPaused)
	{
		if (dragMode)
		{
			return;
		}
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(FVector(0.f,0.f,0.f));

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.bNoFail = true;

		if (towerType == "Cannon")
		{
			if (GoldAvailable(Cannon::goldToBuild))
			{
				spawnedTower = GetWorld()->SpawnActor<ACannonTower>(CannonTowerClass, SpawnTransform, SpawnParams);
				dragMode = true;
				ShowUnusedTowerBases(false);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Not enough gold to spawn Cannon tower")));
			}
		}
		else if (towerType == "Ice")
		{
			if (GoldAvailable(Ice::goldToBuild))
			{
				spawnedTower = GetWorld()->SpawnActor<AIceTower>(IceTowerClass, SpawnTransform, SpawnParams);
				dragMode = true;
				ShowUnusedTowerBases(false);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Not enough gold to spawn Ice tower")));
			}
		}
		//else if (towerType == "...")
	}
}

void ATower_GameMode::FinalizeTowerSpawn()
{
	// check tower position
	// if not ok, destroy tower
	if (selectedBase != nullptr && !selectedBase->used)
	{
		//selectedBase->SetActorHiddenInGame(true);
		ShowUnusedTowerBases(true);
		selectedBase->used = true;
		UpdateGold(-spawnedTower->GetGoldToBuild());
		spawnedTower->Activate();
		spawnedTower->SetTowerBase(selectedBase);
	}
	else
	{
		spawnedTower->Destroy();
	}
	dragMode = false;
	selectedBase = nullptr;
	spawnedTower = nullptr;
}

void ATower_GameMode::IncrementWave()
{
	waveCount++;
	HudWidgetPlayer->SetWaveText();

	//// ogni 30 secondi il moltiplicatore aumenta di 0.5
	healthMultiplier += 0.5f;

	FTimerDelegate TimerDel;
	FTimerHandle TimerHandle;

	TimerDel.BindUFunction(this, FName("StopBetweenWaves"));
	//Calling RestoreMoveSpeed after "time" seconds without looping
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 30.f, false);
}

int32 ATower_GameMode::GetWave()
{
	return waveCount;
}

void ATower_GameMode::StopBetweenWaves()
{
	spawnPoint->StopSpawning(10.f);
	// add text to ui indicating the 10 sec break between a wave and another?
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Waiting 10 seconds before wave %d"), waveCount+1));
}

void ATower_GameMode::GoToMainMenu()
{
	UGameplayStatics::OpenLevel(this, FName(TEXT("/Game/Tower_Defense/Levels/StartMenuLevel")));
}

void ATower_GameMode::ShowUnusedTowerBases(bool toHide)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATowerBase::StaticClass(), FoundActors);
	for (int i = 0; i < FoundActors.Num(); ++i)
	{
		if (!Cast<ATowerBase>(FoundActors[i])->used)
		{
			Cast<ATowerBase>(FoundActors[i])->SetActorHiddenInGame(toHide);
		}
	}
}


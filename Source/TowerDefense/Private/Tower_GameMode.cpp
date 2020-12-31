// Fill out your copyright notice in the Description page of Project Settings.
//#pragma optimize("", off)

#include "Tower_GameMode.h"
#include "Tower_GameState.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Stats_HUD.h"
// spostare in stats_hud
#include "Components/TextBlock.h"
#include "Game_UserWidget.h"


ATower_GameMode::ATower_GameMode() : Super()
{
	MaxHealth = 10;
	HealthDamage = 0.0f;
	MaxGold = 999;

	InitiateGameOver = false;

	MyTimeline = NewObject<UTimelineComponent>(this, FName("Health_Animation"));
}

// Called every frame
void ATower_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MyTimeline != nullptr) MyTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
}

void ATower_GameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogActor, Warning, TEXT("Loaded Tower_GameMode"))

	HandleNewState(EGamePlayState::EPlaying);

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

		Cast<UTextBlock>(HudWidgetPlayer->GetHUDWidget()->GetWidgetFromName(FName("Health_Text")))->SetText(GS->GetHealthText());

		GS->HealthPercentage = CurveFloatValue;
		// if health reaches 0, enter gameover state
		if (GS->Health <= 0)
		{
			MyTimeline->Stop();
			HandleNewState(EGamePlayState::EGameOver);
		}
	}
}

//void ATower_GameMode::OnTargetHit()
//{
//	if (ATower_GameState* GS = Cast<ATower_GameState>(GameState))
//	{
//		GS->Points++;
//
//		if (GS->Points >= PointsToWin)
//		{
//			//UE_LOG(LogTemp, Warning, TEXT("You won the gaem!!!1"));
//			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("You won the game, you took %f seconds to win the game."), GetWorld()->GetTimeSeconds()));
//		}
//		else
//		{
//			//UE_LOG(LogTemp, Warning, TEXT("You scored a point. You now have %d points"), GS->Points);
//			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("You scored a point. You now have %d points."), GS->Points));
//		}
//	}
//}

// add to the player golds, return updated amount
void ATower_GameMode::AddGold(float gold)
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
	//return GS->Gold;

}

void ATower_GameMode::AddToEndPositions(const FVector& fv)
{
	UE_LOG(LogActor, Warning, TEXT("Added End point, coordinates: x: %f, y: %f, z: %f"), fv.X, fv.Y, fv.Z)
	EndPointPosition.push_back(fv);
}

// returns only the first saved position, only 1 end point is supported
FVector ATower_GameMode::GetEndPositions()
{
	return EndPointPosition[0];
}

void ATower_GameMode::HandleNewState(EGamePlayState NewState)
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
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("WE FUCKING LOST!")));
			//UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);

			HudWidgetPlayer->GameOverMode();

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

void ATower_GameMode::GoToMainMenu()
{
	UGameplayStatics::OpenLevel(this, FName(TEXT("/Game/Tower_Defense/Levels/StartMenuLevel")));
}

// Fill out your copyright notice in the Description page of Project Settings.
//#pragma optimize("", off)

#include "Tower_GameMode.h"
#include "Tower_GameState.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"

ATower_GameMode::ATower_GameMode() : Super()
{
	MaxHealth = 10;
	HealthDamage = 0.0f;
	MaxGold = 999;
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

	SetCurrentState(EGamePlayState::EPlaying);

	GS = Cast<ATower_GameState>(GetWorld()->GetGameState());
	if (GS == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("Game State not found!"))
	}
	if (HealthCurve == nullptr)
    {
		UE_LOG(LogActor, Warning, TEXT("HealthCurve not found!"))
	}

	// setting health at the start of the game
	GS->Health = MaxHealth;
	PreviousHealth = GS->HealthPercentage;

	// timeline and callback
    FOnTimelineFloat TimelineCallback;
    TimelineCallback.BindUFunction(this, FName("SetHealth"));
	MyTimeline = NewObject<UTimelineComponent>(this, FName("Health_Animation"));
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
	TimelineValue = MyTimeline->GetPlaybackPosition();
    CurveFloatValue = PreviousHealth + HealthDamage*HealthCurve->GetFloatValue(TimelineValue);
	GS->Health = UKismetMathLibrary::Round(CurveFloatValue*MaxHealth);
	GS->HealthPercentage = CurveFloatValue;
	// if health reaches 0, enter gameover state
	if (GS->Health <= 0)
	{
		SetCurrentState(EGamePlayState::EGameOver);
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
int32 ATower_GameMode::AddGold(float gold)
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
	return GS->Gold;

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

EGamePlayState ATower_GameMode::GetCurrentState() const
{
	return CurrentState;
}

void ATower_GameMode::SetCurrentState(EGamePlayState NewState)
{
	CurrentState = NewState;
	HandleNewState(CurrentState);
}

void ATower_GameMode::HandleNewState(EGamePlayState NewState)
{
	switch (NewState)
	{
		case EGamePlayState::EPlaying:
		{
			// do nothing
		}
		break;
		// Unknown/default state
		case EGamePlayState::EGameOver:
		{
			// restart the level
			UE_LOG(LogActor, Warning, TEXT("WE FUCKING LOST!"))
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("WE FUCKING LOST!")));
			UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
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
// Unreal Engine 4 Tower Defense
// #pragma optimize("", off)

#include "Tower_GameState.h"
#include "Tower_GameMode.h"
#include "Engine/World.h"
#include "GameplayStats.h"

ATower_GameState::ATower_GameState()
{
	Gold = Game::startingGold;
	HealthPercentage = 1.f;
	isPaused = false;
}

void ATower_GameState::BeginPlay()
{
	Super::BeginPlay();

	GM = Cast<ATower_GameMode>(GetWorld()->GetAuthGameMode());
	if (GM == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("Game Mode not found!"))
	}
}

float ATower_GameState::GetHealthPercentage() const
{
	return HealthPercentage;
}

FText ATower_GameState::GetHealthText() const
{
	FString HP = FString::FromInt(Health);
	FText HPText = FText::FromString(HP);
	return HPText;
}

FText ATower_GameState::GetGoldText() const
{
	FString G = FString::FromInt(Gold);
	FText GText = FText::FromString(G);
	return GText;
}

int32 ATower_GameState::GetGold() const
{
	return Gold;
}

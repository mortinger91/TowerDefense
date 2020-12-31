// #pragma optimize("", off)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenu_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API AMainMenu_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainMenu_GameMode();

	virtual void BeginPlay() override;

	virtual void Tick (float DeltaTime) override;

	APlayerController* PC;
};

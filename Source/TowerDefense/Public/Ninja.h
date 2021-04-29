// Unreal Engine 4 Tower Defense

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Ninja.generated.h"

UCLASS()
class TOWERDEFENSE_API ANinja : public AEnemy
{
	GENERATED_BODY()
	
public:
	ANinja();

	void BeginPlay() override;

	void Tick(float DeltaTime) override;
};

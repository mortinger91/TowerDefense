// Unreal Engine 4 Tower Defense

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Tower_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API ATower_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATower_PlayerController();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:
	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//void MoveForward(float Amount);

	//void MoveRight(float Amount);

	//void Turn(float Amount);

	//void LookUp(float Amount);

};

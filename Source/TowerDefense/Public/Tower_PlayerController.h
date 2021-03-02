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
	virtual void SetupInputComponent() override;

	void MoveForward(float Amount);

	void MoveRight(float Amount);

	void Turn(float Amount);

	void LookUp(float Amount);

	void ClickAction(); 
	void ReleaseAction();

	// mobile touch controls
	FVector2D previousTouchLocation1;
	FVector2D newTouchLocation1;
	
	FVector2D touchLocation2;
	float previousDistance;
	float newDistance;

	void OnTouchBegin(ETouchIndex::Type touchType, FVector touchLocation);
	void TouchReleaseAction(ETouchIndex::Type touchType, FVector touchLocation);
	
	//UPROPERTY(EditAnywhere, Category = "Stats")
	//class UUserWidget * CursorWidgetClass;
	
	class ATower_GameMode* GM;

	bool leftMouseIsClicked;

	inline float DistanceOfTwoPoints(const FVector2D& p1, const FVector2D& p2) const;
};

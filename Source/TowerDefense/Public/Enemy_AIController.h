// Unreal Engine 4 Tower Defense

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enemy_AIController.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API AEnemy_AIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemy_AIController();

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* MyPawn) override;

	virtual void Tick (float DeltaTime) override;

	UFUNCTION()
    void GoToRandomEndPoint();

private:
    UPROPERTY()
    TArray<AActor *> EndPoints;

    UFUNCTION()
	//class AEndPoint * GetRandomEndPoint();
	ATargetPoint * GetRandomEndPoint() const;
	
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

};

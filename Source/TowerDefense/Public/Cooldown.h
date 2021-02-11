// Unreal Engine 4 Tower Defense

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Cooldown.generated.h"


//UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
UCLASS()
class TOWERDEFENSE_API UCooldown : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCooldown();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "Cooldown")
	float maxCooldown;

	UFUNCTION()
	bool IsNotCooldown();

	void StartCooldown();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float actualCooldown;
};

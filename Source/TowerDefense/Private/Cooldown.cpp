// Unreal Engine 4 Tower Defense
 //#pragma optimize("", off)

#include "Cooldown.h"
#include "Engine/World.h"

// Sets default values for this component's properties
//UCooldown::UCooldown()
//{
//	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
//	// off to improve performance if you don't need them.
//	//PrimaryComponentTick.bCanEverTick = true;
//
//	//actualCooldown = 0.f;
//}


// Called when the game starts
void UCooldown::BeginPlay()
{
	Super::BeginPlay();
	
	actualCooldown = -maxCooldown;
}

// Called every frame
void UCooldown::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UCooldown::IsNotCooldown()
{
	if ((GetWorld()->GetRealTimeSeconds() - actualCooldown) >= maxCooldown)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UCooldown::StartCooldown()
{
	actualCooldown = GetWorld()->GetRealTimeSeconds();
}


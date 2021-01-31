// Unreal Engine 4 Tower Defense


#include "Target.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATarget::ATarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>("TargetMesh");
	SetRootComponent(TargetMesh);

	MoveScale = 200.f;
}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	Super::BeginPlay();

	MoveDirection = FMath::VRand();
	
}

// Called every frame 

void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector MoveDelta = (MoveDirection * ( FMath::Sin(GetWorld()->GetTimeSeconds() + DeltaTime) - FMath::Sin(GetWorld()->GetTimeSeconds()))) * MoveScale;

	AddActorWorldOffset(MoveDelta, true);
}


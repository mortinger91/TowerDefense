// Unreal Engine 4 Tower Defense


#include "TowerBase.h"

// Sets default values
ATowerBase::ATowerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	TowerBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("TowerBaseMesh");
	SetRootComponent(TowerBaseMesh);

	used = false;
}

// Called when the game starts or when spawned
void ATowerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATowerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


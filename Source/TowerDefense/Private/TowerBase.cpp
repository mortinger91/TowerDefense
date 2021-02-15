// Unreal Engine 4 Tower Defense
// #pragma optimize("", off)

#include "TowerBase.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATowerBase::ATowerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	TowerBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("TowerBaseMesh");
	SetRootComponent(TowerBaseMesh);

	Particles = CreateDefaultSubobject<UParticleSystemComponent>("Particles");
	FAttachmentTransformRules rules(EAttachmentRule::KeepRelative, false);
	Particles->AttachToComponent(RootComponent, rules);

	used = false;
}

// Called when the game starts or when spawned
void ATowerBase::BeginPlay()
{
	Super::BeginPlay();
	
	//Particles->Deactivate();
}

// Called every frame
void ATowerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


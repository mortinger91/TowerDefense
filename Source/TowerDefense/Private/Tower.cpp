// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Cooldown.h"

// Sets default values
ATower::ATower()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>("TowerMesh");

	SetRootComponent(TowerMesh);

	// component cooldown
	cooldownShot = CreateDefaultSubobject<UCooldown>("cooldown");
	cooldownShot->maxCooldown = 5.f;

	shiftSock = 50.f;
	damage = 40.f;
	
}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();
	
	// added ad hoc for the Tower_Merged_mesh
	sockets.Add(TowerMesh->GetSocketLocation(FName("Socket1")) + FVector(shiftSock, 0.f, 0.f) );
	sockets.Add(TowerMesh->GetSocketLocation(FName("Socket2")) + FVector(-shiftSock, 0.f, 0.f) );
	sockets.Add(TowerMesh->GetSocketLocation(FName("Socket3")) + FVector(0.f, -shiftSock, 0.f) );
	sockets.Add(TowerMesh->GetSocketLocation(FName("Socket4")) + FVector(0.f, shiftSock, 0.f) );
}

// Called every frame
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


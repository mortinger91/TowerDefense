// Fill out your copyright notice in the Description page of Project Settings.


#include "EndPoint.h"
#include "Tower_GameMode.h"
#include "Enemy.h"

// Sets default values
AEndPoint::AEndPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EndMesh = CreateDefaultSubobject<UStaticMeshComponent>("EndMesh");

	SetRootComponent(EndMesh);

	OnActorHit.AddDynamic(this, &AEndPoint::RemoveHealth);
}

// Called when the game starts or when spawned
void AEndPoint::BeginPlay()
{
	Super::BeginPlay();

	GM = Cast<ATower_GameMode>(GetWorld()->GetAuthGameMode());
	if (GM == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("No Game Mode found for %s!"), *this->GetName())
	}

	SetEndPointPosition();
	
}

// Called every frame
void AEndPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEndPoint::SetEndPointPosition()
{
	GM->AddToEndPositions(GetActorLocation());
}

void AEndPoint::RemoveHealth(AActor * SelfActor, AActor * OtherActor, FVector NormalImpulse, const FHitResult & Hit)
{
	// se il cast è andato a buon fine, allora l'actor che è stato colpito è un Enemy
	if (AEnemy* Target = Cast<AEnemy>(OtherActor))
	{
		Target->Destroy();
		GM->Remove1Health();
	}
}

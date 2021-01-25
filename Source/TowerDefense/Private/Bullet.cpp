// Fill out your copyright notice in the Description page of Project Settings.
//#pragma optimize("", off)

#include "Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "Tower.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");

	SetRootComponent(BulletMesh);

	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>("BulletMovement");
	BulletMovement->InitialSpeed = 5000.f;
	//BulletMovement->Bounciness = 0.5f;
	BulletMovement->MaxSpeed = 7000.f;

	OnActorHit.AddDynamic(this, &ABullet::OnBulletHit);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::OnBulletHit(AActor * SelfActor, AActor * OtherActor, FVector NormalImpulse, const FHitResult & Hit)
{
	// se il cast è andato a buon fine, allora l'actor che è stato colpito è un Enemy
	if (AEnemy* enemy = Cast<AEnemy>(OtherActor))
	{
		//if (ATower_GameMode* GM = Cast<ATower_GameMode>(GetWorld()->GetAuthGameMode()))
		//{
		//	GM->OnTargetHit();
		//}

		DealDamage(enemy);
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particlesExplosion, GetTransform(), true, EPSCPoolMethod::None, true);
	Destroy();
}

void ABullet::DealDamage(AEnemy * enemy)
{
	float damage;
	if (ATower* tower = Cast<ATower>(GetInstigator()))
	{
		damage = tower->GetDamage();
	}
	else
	{
		damage = 20.f;
	}
	enemy->GetDamaged(damage);
}


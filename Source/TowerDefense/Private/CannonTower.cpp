// Unreal Engine 4 Tower Defense
// #pragma optimize("", off)

#include "CannonTower.h"
#include "GameplayStats.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

ACannonTower::ACannonTower()
{
	// specific cannon tower parameters
	shiftSock = 35.f;

	// GAMEPLAY PARAMETERS
	towerType = "Cannon";
	SetCooldown(Cannon::shotCooldown);
	damage1 = Cannon::damage1;
	damage2 = Cannon::damage2;
	damage3 = Cannon::damage3;
	goldToBuild = Cannon::goldToBuild;
	goldToUpgrade1 = Cannon::goldToUpgrade1;
	goldToUpgrade2 = Cannon::goldToUpgrade2;
	goldToSell1 = Cannon::goldToSell1;
	goldToSell2 = Cannon::goldToSell2;
	goldToSell3 = Cannon::goldToSell3;
	AISightRadius = Cannon::AISightRadius;
}

void ACannonTower::BeginPlay()
{
	Super::BeginPlay();

	if (BulletClass == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("In CannonTower: BulletClass not found!"))
	}

	if (!(sockets.Num() > 0))
	{
		UE_LOG(LogActor, Warning, TEXT("No Tower Sockets found for %s!"), *this->GetName())
	}
}

void ACannonTower::Activate()
{
	// added ad hoc for the Tower_Merged_mesh
	sockets.Add(TowerMesh->GetSocketLocation(FName("Socket0")) + FVector(shiftSock, 0.f, 0.f) );
	sockets.Add(TowerMesh->GetSocketLocation(FName("Socket1")) + FVector(-shiftSock, 0.f, 0.f) );
	sockets.Add(TowerMesh->GetSocketLocation(FName("Socket2")) + FVector(0.f, -shiftSock, 0.f) );
	sockets.Add(TowerMesh->GetSocketLocation(FName("Socket3")) + FVector(0.f, shiftSock, 0.f) );

	Super::Activate();
}

void ACannonTower::Shoot(const AActor* enemyToShoot)
{
	// get closest cannon to enemy location
	FVector enemyLocation;
	{
		if (enemyToShoot == nullptr)
			return;
		enemyLocation = enemyToShoot->GetActorLocation();
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Shooting enemy: %s, location: %f %f %f"), *enemyToShoot->GetName(), enemyLocation.X, enemyLocation.Y, enemyLocation.Z ));
	}
	float min = FVector::Dist(sockets[0], enemyLocation);
	int32 socketIndex = 0;
	for (int32 i = 0; i < sockets.Num(); ++i)
	{
		if (FVector::Dist(sockets[i], enemyLocation) < min)
		{
			min = FVector::Dist(sockets[i], enemyLocation);
			socketIndex = i;
		}
	}
		
	FActorSpawnParameters BulletSpawnParams;
	BulletSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	BulletSpawnParams.bNoFail = true;
	BulletSpawnParams.Owner = this;
	BulletSpawnParams.Instigator = this;

	FTransform BulletSpawnTransform;
	BulletSpawnTransform.SetLocation(sockets[socketIndex]);
	FVector fv = UKismetMathLibrary::InverseTransformLocation(BulletSpawnTransform, enemyLocation);
	FRotator fr = UKismetMathLibrary::Conv_VectorToRotator(fv);
	BulletSpawnTransform.SetRotation(fr.Quaternion()); 
	BulletSpawnTransform.SetScale3D(FVector(0.8f));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particlesShooting, BulletSpawnTransform, true, EPSCPoolMethod::None, true);
	GetWorld()->SpawnActor<ABullet>(BulletClass, BulletSpawnTransform, BulletSpawnParams);
}


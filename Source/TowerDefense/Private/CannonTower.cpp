// Unreal Engine 4 Tower Defense

#include "CannonTower.h"
#include "GameplayStats.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"

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
	if (BulletClass != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.bNoFail = true;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;

		FTransform BulletSpawnTransform;
		
		// atomic
		FVector enemyLocation;
		{
			if (enemyToShoot == nullptr)
				return;
			enemyLocation = enemyToShoot->GetActorLocation();
		}

		float min = FVector::Dist(sockets[0], enemyLocation);
		int32 socketIndex = 0;
		if (!(sockets.Num() > 0))
		{
			UE_LOG(LogActor, Warning, TEXT("No Tower Sockets found for %s!"), *this->GetName())
		}
		for (int32 i = 0; i < sockets.Num(); ++i)
		{
			if (FVector::Dist(sockets[i], enemyLocation) < min)
			{
				min = FVector::Dist(sockets[i], enemyLocation);
				socketIndex = i;
			}
		}
		
		BulletSpawnTransform.SetLocation(sockets[socketIndex]);

		FVector fv = UKismetMathLibrary::InverseTransformLocation(BulletSpawnTransform, enemyLocation);
		FRotator fr = UKismetMathLibrary::Conv_VectorToRotator(fv);
		//BulletSpawnTransform.SetRotation(tower->GetActorRotation().Quaternion()); 
		BulletSpawnTransform.SetRotation(fr.Quaternion()); 
		BulletSpawnTransform.SetScale3D(FVector(0.8f));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particlesShooting, BulletSpawnTransform, true, EPSCPoolMethod::None, true);
		GetWorld()->SpawnActor<ABullet>(BulletClass, BulletSpawnTransform, SpawnParams);
	}
}

//void ACannonTower::BeginPlay()
//{
//	Super::BeginPlay();
//}

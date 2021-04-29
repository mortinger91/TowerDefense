// Unreal Engine 4 Tower Defense


#include "IceTower.h"
#include "GameplayStats.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "IceBullet.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AIceTower::AIceTower()
{
	// GAMEPLAY PARAMETERS
	towerType = "Ice";
	SetCooldown(Ice::shotCooldown);
	damage1 = Ice::slow1;
	damage2 = Ice::slow2;
	damage3 = Ice::slow3;
	slowTime1 = Ice::slowTime1;
	slowTime2 = Ice::slowTime2;
	slowTime3 = Ice::slowTime3;
	goldToBuild = Ice::goldToBuild;
	goldToUpgrade1 = Ice::goldToUpgrade1;
	goldToUpgrade2 = Ice::goldToUpgrade2;
	goldToSell1 = Ice::goldToSell1;
	goldToSell2 = Ice::goldToSell2;
	goldToSell3 = Ice::goldToSell3;
	AISightRadius = Ice::AISightRadius;
}

void AIceTower::BeginPlay()
{
	Super::BeginPlay();

	if (BulletClass == nullptr)
	{
		UE_LOG(LogActor, Warning, TEXT("In IceTower: BulletClass not found!"))
	}
}

void AIceTower::Activate()
{
	socket = TowerMesh->GetSocketLocation(FName("Socket0"));

	Super::Activate();
}

void AIceTower::Shoot(const AActor* enemyToShoot)
{
	FVector enemyLocation;
	{
		if (enemyToShoot == nullptr)
			return;
		enemyLocation = enemyToShoot->GetActorLocation();
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Shooting enemy: %s, location: %f %f %f"), *enemyToShoot->GetName(), enemyLocation.X, enemyLocation.Y, enemyLocation.Z ));
	}

	FActorSpawnParameters BulletSpawnParams;
	BulletSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	BulletSpawnParams.bNoFail = true;
	BulletSpawnParams.Owner = this;
	BulletSpawnParams.Instigator = this;

	FTransform BulletSpawnTransform;
	BulletSpawnTransform.SetLocation(socket);
	FVector fv = UKismetMathLibrary::InverseTransformLocation(BulletSpawnTransform, enemyLocation);
	FRotator fr = UKismetMathLibrary::Conv_VectorToRotator(fv);
	BulletSpawnTransform.SetRotation(fr.Quaternion()); 
	BulletSpawnTransform.SetScale3D(FVector(0.8f));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particlesShooting, BulletSpawnTransform, true, EPSCPoolMethod::None, true);
	GetWorld()->SpawnActor<AIceBullet>(BulletClass, BulletSpawnTransform, BulletSpawnParams);
}

float AIceTower::GetSlowTime() const
{
	switch (level)
	{
		case 1:
		{
			return slowTime1;
		}
		break;
		case 2:
		{
			return slowTime2;
		}
		break;
		case 3:
		{
			return slowTime3;
		}
		break;
	}
	return 0;
}
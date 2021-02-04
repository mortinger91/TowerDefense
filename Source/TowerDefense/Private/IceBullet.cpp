// Unreal Engine 4 Tower Defense
// #pragma optimize("", off)

#include "IceBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "IceTower.h"

// Sets default values
AIceBullet::AIceBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");

	SetRootComponent(BulletMesh);

	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>("BulletMovement");
	BulletMovement->InitialSpeed = 5000.f;
	BulletMovement->MaxSpeed = 7000.f;

	OnActorHit.AddDynamic(this, &AIceBullet::OnBulletHit);
}

// Called when the game starts or when spawned
void AIceBullet::BeginPlay()
{
	Super::BeginPlay();

	tower = Cast<AIceTower>(GetInstigator());
}

// Called every frame
void AIceBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIceBullet::OnBulletHit(AActor * SelfActor, AActor * OtherActor, FVector NormalImpulse, const FHitResult & Hit)
{
	// se il cast è andato a buon fine, allora l'actor che è stato colpito è un Enemy
	if (AEnemy* enemy = Cast<AEnemy>(OtherActor))
	{
		enemy->GetSlowed(tower->GetDamage(), tower->GetSlowTime());
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particlesExplosion, GetTransform(), true, EPSCPoolMethod::None, true);
	Destroy();
}

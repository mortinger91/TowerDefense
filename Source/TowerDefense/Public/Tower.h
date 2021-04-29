// Unreal Engine 4 Tower Defense

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tower.generated.h"

UCLASS(abstract)
class TOWERDEFENSE_API ATower : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATower();

	class UCooldown* cooldownShot;

	virtual void Shoot(const AActor* enemyToShoot) PURE_VIRTUAL(ATower::Shoot, return;);

	float GetDamage() const; //dipende dal livello

	int32 GetLevel() const; // restituisce il livello
	 
	void LevelUp(); // aumenta di livello se possibile
	
	void Sell(); // aumenta di livello se possibile

	int32 GetGoldToUpgrade() const; // restituisce i gold necessari al prossimo upgrade

	int32 GetGoldToSell() const;

	int32 GetGoldToBuild() const;

	FString GetTowerType() const;

	float GetAISightRadius() const;

	virtual void Activate();

	void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;

	FVector towerPosition;

	void SetTowerBase(class ATowerBase* towerBaseToSet);
	class ATowerBase* GetTowerBase() const;

protected:
	//UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UPROPERTY(EditAnywhere, Category = "Static Mesh Component")
	UStaticMeshComponent* TowerMesh;

	//UPROPERTY(EditAnywhere, Category="Capsule component")
	//class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, Category = "Particles")
	class UParticleSystem* spawnParticles;

	UPROPERTY(EditAnywhere, Category = "Particles")
	class UParticleSystem* upgradeParticles;

	UPROPERTY(EditAnywhere, Category = "Particles")
	class UParticleSystem* sellParticles;

	void SetCooldown(float cooldown);

	FString towerType; // il tipo della torre

	int32 level;

	float damage1;
	float damage2;
	float damage3;
	
	int32 goldToBuild;
	int32 goldToUpgrade1;
	int32 goldToUpgrade2;
	int32 goldToSell1;
	int32 goldToSell2;
	int32 goldToSell3;

	float AISightRadius;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class ATower_GameMode* GM;

	class ATowerBase* towerBase;
};

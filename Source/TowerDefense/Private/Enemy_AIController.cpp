// Unreal Engine 4 Tower Defense
// #pragma optimize("", off)

#include "Enemy_AIController.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Tower_GameMode.h"
#include "Engine/World.h"

AEnemy_AIController::AEnemy_AIController()
{
	//PrimaryActorTick.bCanEverTick = true;
}

void AEnemy_AIController::BeginPlay()
{
	Super::BeginPlay();

	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEndPoint::StaticClass(), EndPoints);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), EndPoints);

}

void AEnemy_AIController::OnPossess(APawn* MyPawn)
{
	Super::OnPossess(MyPawn);
}

void AEnemy_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy_AIController::GoToRandomEndPoint()
{
	MoveToActor(GetRandomEndPoint());
}

ATargetPoint * AEnemy_AIController::GetRandomEndPoint()
{
	//auto index = FMath::RandRange(0, Waypoints.Num() - 1);
	return Cast<ATargetPoint>(EndPoints[0]);
}

void AEnemy_AIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result)
{
    Super::OnMoveCompleted(RequestID, Result);

	UE_LOG(LogActor, Warning, TEXT("In Enemy_AIController: MoveCompleted!"))
	Cast<ATower_GameMode>(GetWorld()->GetAuthGameMode())->Remove1Health();
	GetPawn()->Destroy();
	//Destroy();
}

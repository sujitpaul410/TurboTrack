#include "TrackObstacle.h"

#include "PlayerPawn.h"


ATrackObstacle::ATrackObstacle()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>("ObstacleSplineComponent");
	if (SplineComponent)
	{
		SetRootComponent(SplineComponent);
	}
}

void ATrackObstacle::OnMeshOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult)
{
	// UE_LOG(LogTemp, Warning, TEXT("Overlapping Actor: %s"), *OtherComp->GetName());
	if (OtherActor && OtherActor->IsA(APlayerPawn::StaticClass()))
	{
		// UE_LOG(LogTemp, Warning, TEXT("Player overlapping Lazy mesh!"));
		TotalOverlapped++;
		OnLazySplineOverlap.Broadcast();
	}
}

void ATrackObstacle::OnMeshOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA(APlayerPawn::StaticClass()))
	{
		// UE_LOG(LogTemp, Warning, TEXT("Player ended overlapping nitro mesh!"));
		// OnNitroSplineOverlapEnd.Broadcast();

		TotalOverlapped--;
		if (TotalOverlapped == 0)
		{
			// UE_LOG(LogTemp, Warning, TEXT("Player overlapping ended lazy mesh!"));
			OnLazySplineOverlapEnd.Broadcast();
		}
	}
}

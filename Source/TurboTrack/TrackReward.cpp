#include "TrackReward.h"

#include "PlayerPawn.h"


ATrackReward::ATrackReward()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>("RewardSplineComponent");
	if (SplineComponent)
	{
		SetRootComponent(SplineComponent);
	}
}



void ATrackReward::OnMeshOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// UE_LOG(LogTemp, Warning, TEXT("Overlapping Actor: %s"), *OtherComp->GetName());
	if (OtherActor && OtherActor->IsA(APlayerPawn::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player overlapping nitro mesh!"));
		TotalOverlapped++;
		OnNitroSplineOverlap.Broadcast();
	}
}

void ATrackReward::OnMeshOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA(APlayerPawn::StaticClass()))
	{
		// UE_LOG(LogTemp, Warning, TEXT("Player ended overlapping nitro mesh!"));
		// OnNitroSplineOverlapEnd.Broadcast();
		
		TotalOverlapped--;
		if (TotalOverlapped == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player overlapping ended nitro mesh!"));
			OnNitroSplineOverlapEnd.Broadcast();
		}
	}
}


#include "TrackElementSpawner.h"

#include "PlayerPawn.h"
#include "TrackGenerator.h"
#include "TrackReward.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"


ATrackElementSpawner::ATrackElementSpawner()
{
 	
	PrimaryActorTick.bCanEverTick = true;

}

void ATrackElementSpawner::BeginPlay()
{
	Super::BeginPlay();

	//TEST auto spawn nitro track
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATrackElementSpawner::SpawnNitroTrack, 5.0f, true);

	
	PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void ATrackElementSpawner::SpawnNitroTrack()
{
	if (!TrackGenerator || !TrackGenerator->SplineComponent || !RewardClass)
		return;

	USplineComponent* MainSpline = TrackGenerator->SplineComponent;
	
	int MidSegment = MainSpline->GetNumberOfSplinePoints() / 2;
	float MidDistance = MainSpline->GetDistanceAlongSplineAtSplinePoint(MidSegment);

	FVector MidLoc = MainSpline->GetLocationAtDistanceAlongSpline(MidDistance, ESplineCoordinateSpace::World);
	
	FActorSpawnParameters SpawnParams;
	if (Reward != nullptr)
	{
		Reward->OnNitroSplineOverlap.RemoveDynamic(PlayerPawn, &APlayerPawn::OnNitro);
		Reward->OnNitroSplineOverlapEnd.RemoveDynamic(PlayerPawn, &APlayerPawn::OnNitroEnd);
		Reward->Destroy();
	}
	Reward = GetWorld()->SpawnActor<ATrackReward>(RewardClass, MidLoc, FRotator::ZeroRotator, SpawnParams);

	float SideOffset = FMath::RandRange(-1, 1) * SideOffsetAmount;
	
	FVector CurrLoc = Reward->GetActorLocation();
	CurrLoc.Y += SideOffset;
	Reward->SetActorLocation(CurrLoc);
	
	if (Reward)
	{
		float AheadDistance = MidDistance;
		float MaxDistance = MainSpline->GetSplineLength();

		for (int i = 0; i < NitroTrackMaxSplinePoints; ++i)
		{
			if (AheadDistance > MaxDistance)
			{
				UE_LOG(LogTemp, Warning, TEXT("Nito Track breaking"));
				break;
			}

			FVector Loc = MainSpline->GetLocationAtDistanceAlongSpline(AheadDistance, ESplineCoordinateSpace::World);
			Loc.Z += NitroTrackZOffset;
			Loc.Y += SideOffset;
			FVector Tangent = MainSpline->GetTangentAtDistanceAlongSpline(AheadDistance, ESplineCoordinateSpace::World).GetSafeNormal() * NitroTrackSegmentLength;
			Reward->AddSplinePoint(Loc, Tangent);

			AheadDistance += NitroTrackSegmentLength;
		}
	}

	Reward->OnNitroSplineOverlap.AddDynamic(PlayerPawn, &APlayerPawn::OnNitro);
	Reward->OnNitroSplineOverlapEnd.AddDynamic(PlayerPawn, &APlayerPawn::OnNitroEnd);
}

void ATrackElementSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


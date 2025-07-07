#include "TrackElementSpawner.h"

#include "PlayerPawn.h"
#include "TrackGenerator.h"
#include "TrackObstacle.h"
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
	
	FTimerHandle TrackElementTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TrackElementTimerHandle, this, &ATrackElementSpawner::SpawnTrackTypeElement, 4.0f, true);

	FTimerHandle ObstacleElementTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ObstacleElementTimerHandle, this, &ATrackElementSpawner::SpawnNonTrackTypeElement, 15.0f, true);

	
	PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void ATrackElementSpawner::SpawnTrackTypeElement()
{
	int RandomNumber = FMath::RandRange(1, 2);
	
	if (RandomNumber == 1)
	{
		SpawnNitroTrack();
	}
	else if (RandomNumber == 2)
	{
		SpawnLazyTrack();
	}
}

void ATrackElementSpawner::SpawnNonTrackTypeElement()
{
	int RandomNumber = FMath::RandRange(1, 2);
	
	if (RandomNumber == 1)
	{
		SpawnHitableElement(HammerObstacle, HammerObstacleBlueprint, HammerZOffset);
	}
	else if (RandomNumber == 2)
	{
		SpawnHitableElement(ConeObstacle, ConeObstacleBlueprint, ConeZOffset, true);
	}
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

void ATrackElementSpawner::SpawnLazyTrack()
{
	if (!TrackGenerator || !TrackGenerator->SplineComponent || !RewardClass)
		return;

	USplineComponent* MainSpline = TrackGenerator->SplineComponent;
	
	int MidSegment = MainSpline->GetNumberOfSplinePoints() / 2;
	float MidDistance = MainSpline->GetDistanceAlongSplineAtSplinePoint(MidSegment);

	FVector MidLoc = MainSpline->GetLocationAtDistanceAlongSpline(MidDistance, ESplineCoordinateSpace::World);
	
	FActorSpawnParameters SpawnParams;
	if (Obstacle != nullptr)
	{
		Obstacle->OnLazySplineOverlap.RemoveDynamic(PlayerPawn, &APlayerPawn::OnLazy);
		Obstacle->OnLazySplineOverlapEnd.RemoveDynamic(PlayerPawn, &APlayerPawn::OnLazyEnd);
		Obstacle->Destroy();
	}
	Obstacle = GetWorld()->SpawnActor<ATrackObstacle>(ObstacleClass, MidLoc, FRotator::ZeroRotator, SpawnParams);

	float SideOffset = FMath::RandRange(-1, 1) * SideOffsetAmount;
	
	FVector CurrLoc = Obstacle->GetActorLocation();
	CurrLoc.Y += SideOffset;
	Obstacle->SetActorLocation(CurrLoc);
	
	if (Obstacle)
	{
		float AheadDistance = MidDistance;
		float MaxDistance = MainSpline->GetSplineLength();

		for (int i = 0; i < LazyTrackMaxSplinePoints; ++i)
		{
			if (AheadDistance > MaxDistance)
			{
				UE_LOG(LogTemp, Warning, TEXT("Lazy Track breaking"));
				break;
			}

			FVector Loc = MainSpline->GetLocationAtDistanceAlongSpline(AheadDistance, ESplineCoordinateSpace::World);
			Loc.Z += LazyTrackZOffset;
			Loc.Y += SideOffset;
			FVector Tangent = MainSpline->GetTangentAtDistanceAlongSpline(AheadDistance, ESplineCoordinateSpace::World).GetSafeNormal() * LazyTrackSegmentLength;
			Obstacle->AddSplinePoint(Loc, Tangent);

			AheadDistance += LazyTrackSegmentLength;
		}
	}

	Obstacle->OnLazySplineOverlap.AddDynamic(PlayerPawn, &APlayerPawn::OnLazy);
	Obstacle->OnLazySplineOverlapEnd.AddDynamic(PlayerPawn, &APlayerPawn::OnLazyEnd);
}

void ATrackElementSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrackElementSpawner::SpawnHitableElement(AActor*& Obj, TSubclassOf<AActor>& BlueprintClass, float ZOffset, bool bIsSideOffsetRequired)
{
	UE_LOG(LogTemp, Warning, TEXT("Hitable Element"));
	if (!TrackGenerator || !TrackGenerator->SplineComponent || !RewardClass)
		return;

	USplineComponent* MainSpline = TrackGenerator->SplineComponent;
	
	int MidSegment = MainSpline->GetNumberOfSplinePoints() - 2;
	float MidDistance = MainSpline->GetDistanceAlongSplineAtSplinePoint(MidSegment);

	FVector MidLoc = MainSpline->GetLocationAtDistanceAlongSpline(MidDistance, ESplineCoordinateSpace::World);
	MidLoc.Z += ZOffset;
	FVector Tangent = MainSpline->GetTangentAtDistanceAlongSpline(MidDistance, ESplineCoordinateSpace::World).GetSafeNormal();
	
	if (Obj != nullptr)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Hitable Element Destroyed"));
		Obj->Destroy();
	}
	FActorSpawnParameters SpawnParams;

	if (bIsSideOffsetRequired)
	{
		float SideOffset = FMath::RandRange(-1, 1) * SideOffsetAmount;
		MidLoc.Y += SideOffset;
		Obj = GetWorld()->SpawnActor<AActor>(BlueprintClass, MidLoc, Tangent.Rotation(), SpawnParams);
	}
	else
	{
		Obj = GetWorld()->SpawnActor<AActor>(BlueprintClass, MidLoc, Tangent.Rotation(), SpawnParams);
	}
}


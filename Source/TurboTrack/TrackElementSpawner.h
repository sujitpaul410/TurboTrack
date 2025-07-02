#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrackElementSpawner.generated.h"

UCLASS()
class TURBOTRACK_API ATrackElementSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ATrackElementSpawner();

protected:
	
	virtual void BeginPlay() override;
	class APlayerPawn* PlayerPawn;
	class ATrackReward* Reward;

public:
	
	UPROPERTY(EditInstanceOnly, Category="Track")
	class ATrackGenerator* TrackGenerator;

	UPROPERTY(EditAnywhere, Category = "Track")
	TSubclassOf<class ATrackReward> RewardClass;

	UPROPERTY(EditAnywhere, Category = "Track")
	float SideOffsetAmount = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Track")
	float NitroTrackSegmentLength = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Track")
	float NitroTrackZOffset = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Track")
	int NitroTrackMaxSplinePoints = 6;
	
	void SpawnNitroTrack();

	virtual void Tick(float DeltaTime) override;

};

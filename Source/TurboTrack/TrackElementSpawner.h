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
	class ATrackObstacle* Obstacle;
	AActor* HammerObstacle;
	AActor* ConeObstacle;

	void SpawnTrackTypeElement();
	void SpawnNonTrackTypeElement();

public:
	
	UPROPERTY(EditInstanceOnly, Category="Track")
	class ATrackGenerator* TrackGenerator;

	UPROPERTY(EditAnywhere, Category = "Track")
	TSubclassOf<class ATrackReward> RewardClass;
	
	UPROPERTY(EditAnywhere, Category = "Track")
	TSubclassOf<class ATrackObstacle> ObstacleClass;

	UPROPERTY(EditAnywhere, Category = "Track")
	float SideOffsetAmount = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Track")
	float NitroTrackSegmentLength = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Track")
	float NitroTrackZOffset = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Track")
	int NitroTrackMaxSplinePoints = 6;
	
	UPROPERTY(EditAnywhere, Category = "Track")
	int LazyTrackMaxSplinePoints = 6;

	UPROPERTY(EditAnywhere, Category = "Track")
	float LazyTrackZOffset = 20.0f;
	
	UPROPERTY(EditAnywhere, Category = "Track")
	float LazyTrackSegmentLength = 600.0f;
	
	void SpawnNitroTrack();
	void SpawnLazyTrack();

	virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(EditAnywhere, Category = "Obstacle")
	TSubclassOf<AActor> HammerObstacleBlueprint;
	
	UPROPERTY(EditAnywhere, Category = "Obstacle")
	TSubclassOf<AActor> ConeObstacleBlueprint;
	
	UPROPERTY(EditAnywhere, Category = "Obstacle")
	float HammerZOffset = 1250.0f;
	
	UPROPERTY(EditAnywhere, Category = "Obstacle")
	float ConeZOffset = 12.0f;
	
	void SpawnHitableElement(AActor*& Obj, TSubclassOf<AActor>& BlueprintClass, float ZOffset, bool bIsSideOffsetRequired = false);

};

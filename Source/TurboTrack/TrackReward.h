#pragma once

#include "CoreMinimal.h"
#include "TrackElementBase.h"
#include "TrackReward.generated.h"

UCLASS()
class TURBOTRACK_API ATrackReward : public ATrackElementBase
{
	GENERATED_BODY()

private:
	// virtual void PopulateSplineMeshes() override;
	int TotalOverlapped = 0;

public:
	ATrackReward();

	virtual void OnMeshOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                           bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnMeshOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNitroSplineOverlapSignature);

	UPROPERTY(BlueprintAssignable)
	FOnNitroSplineOverlapSignature OnNitroSplineOverlap;


	UPROPERTY(BlueprintAssignable)
	FOnNitroSplineOverlapSignature OnNitroSplineOverlapEnd;
};

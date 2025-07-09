#pragma once

#include "CoreMinimal.h"
#include "TrackElementBase.h"
#include "TrackObstacle.generated.h"


UCLASS()
class TURBOTRACK_API ATrackObstacle : public ATrackElementBase
{
	GENERATED_BODY()

private:
	int TotalOverlapped = 0;

public:
	ATrackObstacle();

	virtual void OnMeshOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                           bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnMeshOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLazySplineOverlapSignature);

	UPROPERTY(BlueprintAssignable)
	FOnLazySplineOverlapSignature OnLazySplineOverlap;


	UPROPERTY(BlueprintAssignable)
	FOnLazySplineOverlapSignature OnLazySplineOverlapEnd;
};

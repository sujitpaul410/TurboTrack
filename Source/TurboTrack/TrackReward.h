#pragma once

#include "CoreMinimal.h"
#include "TrackElementBase.h"
#include "TrackReward.generated.h"

UCLASS()
class TURBOTRACK_API ATrackReward : public ATrackElementBase
{
	GENERATED_BODY()

private:
	void PopulateSplineMeshes();
	
public:
	ATrackReward();
	virtual void OnConstruction(const FTransform& Transform) override;
		
	virtual void OnPlayerHit(AActor* PlayerActor) override;
	virtual void AddSplinePoint(FVector& Location, FVector& Tangent) override;
	virtual void Deactivate() override;
	virtual void InitializeAt(FVector& Location, FVector& Tangent) override;
};

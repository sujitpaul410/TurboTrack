#pragma once

#include "CoreMinimal.h"
#include "TrackElementBase.h"
#include "TrackObstacle.generated.h"


UCLASS()
class TURBOTRACK_API ATrackObstacle : public ATrackElementBase
{
	GENERATED_BODY()

public:
	virtual void OnPlayerHit(AActor* PlayerActor) override;
};

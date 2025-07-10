#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrackMonitor.generated.h"

UCLASS()
class TURBOTRACK_API ATrackMonitor : public AActor
{
	GENERATED_BODY()
	
public:
	ATrackMonitor();
	
	UPROPERTY(EditInstanceOnly, Category="Track")
	class ATrackGenerator* TrackGenerator;

	UPROPERTY(EditAnywhere, Category="Track")
	float MaxAllowedDistance = 1500.0f;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	bool IsPlayerOffTrack();

};

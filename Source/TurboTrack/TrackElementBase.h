#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/Actor.h"
#include "TrackElementBase.generated.h"

UCLASS(Abstract)
class TURBOTRACK_API ATrackElementBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ATrackElementBase();
	
protected:
	
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, Category = "Spline", meta = (AllowPrivateAccess = "true"))
	USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	TEnumAsByte<ESplineMeshAxis::Type> ForwardAxis;

	UPROPERTY(EditAnywhere, Category = "Road")
	float SegmentLength = 1000.f;

	// UPROPERTY(VisibleAnywhere)
	// class UBoxComponent* CollisionBox;

public:
	virtual void InitializeAt(FVector& Location, FVector& Tangent);
	virtual void OnPlayerHit(AActor* PlayerActor);
	virtual void Deactivate();
	
	
	UFUNCTION()
	virtual void AddSplinePoint(FVector& Location, FVector& Tangent);

};

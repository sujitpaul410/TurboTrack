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
	virtual void PopulateSplineMeshes();
	
	UFUNCTION()
	virtual void OnMeshOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
					   bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void OnMeshOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

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
	UFUNCTION()
	virtual void AddSplinePoint(FVector& Location, FVector& Tangent);
};

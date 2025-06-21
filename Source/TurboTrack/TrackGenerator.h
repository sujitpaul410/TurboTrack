// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "TrackGenerator.generated.h"

UCLASS()
class TURBOTRACK_API ATrackGenerator : public AActor
{
	GENERATED_BODY()
	
public:
	ATrackGenerator();
	
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, Category = "Spline")
	USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	TEnumAsByte<ESplineMeshAxis::Type> ForwardAxis;

	//TEST
	UFUNCTION(CallInEditor, Category = "Road")
	void AddSplinePoint(); // Can be called in editor for testing
	//TEST
	UPROPERTY(EditAnywhere, Category = "Road")
	float SegmentLength = 1000.f;

protected:
	virtual void BeginPlay() override;

private:
	FVector RoadEnd;
	APawn* PlayerPawn;

	TArray<USplineMeshComponent*> PooledSplineMeshes;
	
	UPROPERTY(EditAnywhere, Category = "Road")
	float MinDistance=5000.0f;

	void CheckForRoadUpdate();

};

// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackGenerator.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ATrackGenerator::ATrackGenerator()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	if (SplineComponent)
	{
		SetRootComponent(SplineComponent);
	}

}

void ATrackGenerator::OnConstruction(const FTransform& Transform)
{
	if (!Mesh)
	{
		return;
	}

	// Reusing previously stored components
	if (PooledSplineMeshes.Num() == 0)
	{
		GetComponents(PooledSplineMeshes); // Only once
	}

	const int RequiredCount = SplineComponent->GetNumberOfSplinePoints() - 1;

	// Ensuring we have enough pooled mesh components
	while (PooledSplineMeshes.Num() < RequiredCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("Creating Spline Meshes"));
		USplineMeshComponent* NewSplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		NewSplineMesh->SetMobility(EComponentMobility::Movable);
		NewSplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		NewSplineMesh->RegisterComponentWithWorld(GetWorld());
		NewSplineMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
		NewSplineMesh->SetStaticMesh(Mesh);
		NewSplineMesh->SetForwardAxis(ForwardAxis);
		NewSplineMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		
		PooledSplineMeshes.Add(NewSplineMesh);
	}

	// Updating all required spline meshes
	for (int i = 0; i < RequiredCount; ++i)
	{
		USplineMeshComponent* SplineMeshComponent = PooledSplineMeshes[i];
		const FVector StartPoint = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const FVector EndPoint = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
		const FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

		SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
		SplineMeshComponent->SetVisibility(true);
	}

	// Hiding unused spline mesh components
	for (int i = RequiredCount; i < PooledSplineMeshes.Num(); ++i)
	{
		if (PooledSplineMeshes[i])
		{
			PooledSplineMeshes[i]->SetVisibility(false);
		}
	}
}

void ATrackGenerator::AddSplinePoint()
{
	// Extend road forward in X with a slight random curve
	FVector NewLocation = RoadEnd + FVector(SegmentLength, FMath::RandRange(-300.f, 300.f), FMath::RandRange(-200.f, 200.f));
	SplineComponent->AddSplinePoint(NewLocation, ESplineCoordinateSpace::Local);

	RoadEnd = NewLocation;
	if (SplineComponent->GetNumberOfSplinePoints() >= 10)
	{
		SplineComponent->RemoveSplinePoint(0, true);
	}
	
	SplineComponent->UpdateSpline();

	// Recalculate mesh deformation
	OnConstruction(GetActorTransform());
	
}

// Called when the game starts or when spawned
void ATrackGenerator::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	RoadEnd = SplineComponent->GetLocationAtSplinePoint(SplineComponent->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::Local);
}

void ATrackGenerator::CheckForRoadUpdate()
{
	if (PlayerPawn)
	{
		FVector PlayerLocation = PlayerPawn->GetActorLocation();
		float Distance = FVector::Dist(PlayerLocation, RoadEnd);

		if (Distance < MinDistance)
		{
			UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), Distance);
			AddSplinePoint();
		}
	}
}

// Called every frame
void ATrackGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckForRoadUpdate();
}


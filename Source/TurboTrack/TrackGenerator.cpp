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
	TArray<USplineMeshComponent*> Meshes;
	GetComponents(Meshes);

	for (USplineMeshComponent* mesh : Meshes)
	{
		mesh->DestroyComponent();
	}
	
	for (int SplineCount = 0; SplineCount < (SplineComponent->GetNumberOfSplinePoints() - 1); SplineCount++)
	{
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());

		SplineMeshComponent->SetStaticMesh(Mesh);
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->CreationMethod=EComponentCreationMethod::UserConstructionScript;
		SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
		SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

		const FVector StartPoint=SplineComponent->GetLocationAtSplinePoint(SplineCount, ESplineCoordinateSpace::Local);
		const FVector StartTangent=SplineComponent->GetTangentAtSplinePoint(SplineCount, ESplineCoordinateSpace::Local);
		const FVector EndPoint=SplineComponent->GetLocationAtSplinePoint(SplineCount+1, ESplineCoordinateSpace::Local);
		const FVector EndTangent=SplineComponent->GetTangentAtSplinePoint(SplineCount+1, ESplineCoordinateSpace::Local);

		SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);

		SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		SplineMeshComponent->SetForwardAxis(ForwardAxis);
	}
}

void ATrackGenerator::AddSplinePoint()
{
	// Extend road forward in X with a slight random curve
	int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
	FVector LastLocation = SplineComponent->GetLocationAtSplinePoint(NumPoints - 1, ESplineCoordinateSpace::Local);
	FVector NewLocation = LastLocation + FVector(SegmentLength, FMath::RandRange(-300.f, 300.f), FMath::RandRange(-200.f, 200.f));
	SplineComponent->AddSplinePoint(NewLocation, ESplineCoordinateSpace::Local);
	// SplineComponent->UpdateSpline();

	// Recalculate mesh deformation
	// OnConstruction(GetActorTransform());

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
}

void ATrackGenerator::CheckForRoadUpdate()
{
	if (PlayerPawn)
	{
		FVector PlayerLocation = PlayerPawn->GetActorLocation();
		float Distance = FVector::Dist(PlayerLocation, RoadEnd);
		UE_LOG(LogTemp, Log, TEXT("Distance: %f"), Distance);

		if (Distance < MinDistance)
		{
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


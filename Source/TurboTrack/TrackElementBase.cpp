#include "TrackElementBase.h"


ATrackElementBase::ATrackElementBase()
{
}

void ATrackElementBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// UE_LOG(LogTemp, Warning, TEXT("OnConstruction: %s"), *GetNameSafe(SplineComponent));
	TArray<USplineMeshComponent*> OldMeshes;
	GetComponents(OldMeshes);
	for (USplineMeshComponent* Comp : OldMeshes)
	{
		Comp->DestroyComponent(); // Clear old ones if not needed
	}

	if (!SplineComponent)
	{
		// UE_LOG(LogTemp, Error, TEXT("Spline component not found"));
		return;
	}

	PopulateSplineMeshes();
}

void ATrackElementBase::PopulateSplineMeshes()
{
	const int RequiredCount = SplineComponent->GetNumberOfSplinePoints() - 1;
	// UE_LOG(LogTemp, Warning, TEXT("Reward Spline Mesh Size: %d"), RequiredCount);
	for (int i = 0; i < RequiredCount; ++i)
	{
		USplineMeshComponent* NewSplineMesh = NewObject<
			USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		NewSplineMesh->SetMobility(EComponentMobility::Movable);
		NewSplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		NewSplineMesh->RegisterComponentWithWorld(GetWorld());
		NewSplineMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
		NewSplineMesh->SetStaticMesh(Mesh);
		NewSplineMesh->SetForwardAxis(ForwardAxis);
		NewSplineMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		// NewSplineMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
		NewSplineMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
		NewSplineMesh->SetGenerateOverlapEvents(true);
		NewSplineMesh->OnComponentBeginOverlap.AddDynamic(this, &ATrackElementBase::OnMeshOverlap);
		NewSplineMesh->OnComponentEndOverlap.AddDynamic(this, &ATrackElementBase::OnMeshOverlapEnd);

		const FVector StartPoint = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const FVector EndPoint = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
		const FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

		NewSplineMesh->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
		NewSplineMesh->SetVisibility(true);
	}
}

void ATrackElementBase::OnMeshOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	// To be overridden in child classes
}

void ATrackElementBase::OnMeshOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// To be overridden in child classes
}


void ATrackElementBase::AddSplinePoint(FVector& Location, FVector& Tangent)
{
	int Index = SplineComponent->GetNumberOfSplinePoints();

	SplineComponent->AddSplinePoint(Location, ESplineCoordinateSpace::World);
	SplineComponent->SetTangentAtSplinePoint(Index, Tangent, ESplineCoordinateSpace::World);

	SplineComponent->UpdateSpline();

	// OnConstruction(GetActorTransform());

	PopulateSplineMeshes();
}

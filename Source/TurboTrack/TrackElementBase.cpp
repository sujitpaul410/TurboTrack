#include "TrackElementBase.h"


ATrackElementBase::ATrackElementBase()
{
}

void ATrackElementBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ATrackElementBase::InitializeAt(FVector& Location, FVector& Tangent)
{
	// Default implementation
	SetActorLocation(Location);
	SetActorRotation(Tangent.Rotation());
}

void ATrackElementBase::OnPlayerHit(AActor* PlayerActor)
{
	// To be overridden in child classes
}

void ATrackElementBase::Deactivate()
{
	// Default implementation
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

void ATrackElementBase::AddSplinePoint(FVector& Location, FVector& Tangent)
{
	// To be overridden in child classes
}



#include "TrackReward.h"

#include "PlayerPawn.h"

// void ATrackReward::PopulateSplineMeshes()
// {
// 	const int RequiredCount = SplineComponent->GetNumberOfSplinePoints() - 1;
// 	UE_LOG(LogTemp, Warning, TEXT("Reward Spline Mesh Size: %d"), RequiredCount);
// 	for (int i = 0; i < RequiredCount; ++i)
// 	{
// 		USplineMeshComponent* NewSplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
// 		NewSplineMesh->SetMobility(EComponentMobility::Movable);
// 		NewSplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
// 		NewSplineMesh->RegisterComponentWithWorld(GetWorld());
// 		NewSplineMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
// 		NewSplineMesh->SetStaticMesh(Mesh);
// 		NewSplineMesh->SetForwardAxis(ForwardAxis);
// 		NewSplineMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
// 		// NewSplineMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
// 		NewSplineMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
// 		NewSplineMesh->SetGenerateOverlapEvents(true);
// 		NewSplineMesh->OnComponentBeginOverlap.AddDynamic(this, &ATrackReward::OnMeshOverlap);
// 		NewSplineMesh->OnComponentEndOverlap.AddDynamic(this, &ATrackReward::OnMeshOverlapEnd);
// 		
// 		const FVector StartPoint = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
// 		const FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
// 		const FVector EndPoint = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
// 		const FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
// 	
// 		NewSplineMesh->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
// 		NewSplineMesh->SetVisibility(true);
// 	}
// }

ATrackReward::ATrackReward()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>("RewardSplineComponent");
	if (SplineComponent)
	{
		SetRootComponent(SplineComponent);
	}
}

// void ATrackReward::OnConstruction(const FTransform& Transform)
// {
// 	Super::OnConstruction(Transform);
// 	UE_LOG(LogTemp, Warning, TEXT("OnConstruction: %s"), *GetNameSafe(SplineComponent));
// 	TArray<USplineMeshComponent*> OldMeshes;
// 	GetComponents(OldMeshes);
// 	for (USplineMeshComponent* Comp : OldMeshes)
// 	{
// 		Comp->DestroyComponent(); // Clear old ones if not needed
// 	}
//
// 	if (!SplineComponent)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("Spline component not found"));
// 		return;
// 	}
//
// 	PopulateSplineMeshes();
// }
//
//
// void ATrackReward::AddSplinePoint(FVector& Location, FVector& Tangent)
// {	
// 	int Index = SplineComponent->GetNumberOfSplinePoints();
//
// 	SplineComponent->AddSplinePoint(Location, ESplineCoordinateSpace::World);
// 	SplineComponent->SetTangentAtSplinePoint(Index, Tangent, ESplineCoordinateSpace::World);
// 	
// 	SplineComponent->UpdateSpline();
//
// 	// OnConstruction(GetActorTransform());
//
// 	PopulateSplineMeshes();
//
// }


void ATrackReward::OnMeshOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// UE_LOG(LogTemp, Warning, TEXT("Overlapping Actor: %s"), *OtherComp->GetName());
	if (OtherActor && OtherActor->IsA(APlayerPawn::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player overlapping nitro mesh!"));
		TotalOverlapped++;
		OnNitroSplineOverlap.Broadcast();
	}
}

void ATrackReward::OnMeshOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA(APlayerPawn::StaticClass()))
	{
		// UE_LOG(LogTemp, Warning, TEXT("Player ended overlapping nitro mesh!"));
		// OnNitroSplineOverlapEnd.Broadcast();
		
		TotalOverlapped--;
		if (TotalOverlapped == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player overlapping ended nitro mesh!"));
			OnNitroSplineOverlapEnd.Broadcast();
		}
	}
}


#include "Interactable.h"

#include "PlayerPawn.h"

AInteractable::AInteractable()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AInteractable::BeginPlay()
{
	Super::BeginPlay();

	// MeshComponent = Cast<UStaticMeshComponent>(GetRootComponent()->GetChildComponent(0));
	TArray<UStaticMeshComponent*> MeshComponents;
	GetComponents<UStaticMeshComponent>(MeshComponents);

	for (UStaticMeshComponent* Mesh : MeshComponents)
	{
		if (Mesh && Mesh->ComponentHasTag(TEXT("MainMesh")))
		{
			MeshComponent = Mesh;
			break;
		}
	}

	if (MeshComponent && PickupType == EPickupType::Nitro)
	{
		MeshComponent->SetGenerateOverlapEvents(true);
		MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AInteractable::OnMeshOverlap);
	}
	else if (MeshComponent && PickupType == EPickupType::Trash)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Listening to OnHit"));
		MeshComponent->SetNotifyRigidBodyCollision(true);
		MeshComponent->OnComponentHit.AddDynamic(this, &AInteractable::OnComponentHit);
	}

	// if (!MeshComponent)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Mesh Comp is null"));
	// }
	// if (PickupType != EPickupType::Trash)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Pickup type is not Trash"));
	// }
}

void AInteractable::OnMeshOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("On Super Nitro Overlap"));
	OnSuperNitroPickupOverlap.Broadcast();

	Destroy();
}

void AInteractable::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor->IsA(APlayerPawn::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("On Trash Hit"));
		OnTrashPickupHit.Broadcast();
		MeshComponent->OnComponentHit.RemoveDynamic(this, &AInteractable::OnComponentHit);
	}
}

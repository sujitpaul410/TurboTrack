#include "Interactable.h"

#include "MainScreenUserWidget.h"
#include "PlayerPawn.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

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
	else if (MeshComponent && PickupType == EPickupType::Hammer)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Listening to HammerHit"));
		MeshComponent->SetNotifyRigidBodyCollision(true);
		MeshComponent->OnComponentHit.AddDynamic(this, &AInteractable::OnHammerHit);
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
	// UE_LOG(LogTemp, Warning, TEXT("On Super Nitro Overlap"));
	UGameplayStatics::PlaySound2D(this, NitroSoundCue, 0.3);
	OnSuperNitroPickupOverlap.Broadcast();

	Destroy();
}

void AInteractable::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor->IsA(APlayerPawn::StaticClass()))
	{
		// UE_LOG(LogTemp, Warning, TEXT("On Trash Hit"));
		UGameplayStatics::PlaySound2D(this, TrashSoundCue);
		OnTrashPickupHit.Broadcast();
		MeshComponent->OnComponentHit.RemoveDynamic(this, &AInteractable::OnComponentHit);
	
		TArray<UUserWidget*> FoundWidgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UMainScreenUserWidget::StaticClass(), false);

		if (FoundWidgets.Num() > 0)
		{
			UMainScreenUserWidget* Found = Cast<UMainScreenUserWidget>(FoundWidgets[0]);
			if (Found)
			{
				Found->OnObstacleHit();
			}
		}
	}
}

void AInteractable::OnHammerHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::PlaySound2D(this, HammerSoundCue);
}

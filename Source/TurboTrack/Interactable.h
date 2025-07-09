#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UENUM(BlueprintType)
enum class EPickupType : uint8
{
	Trash UMETA(DisplayName = "Trash"),
	Nitro UMETA(DisplayName = "Nitro"),
	Hammer UMETA(DisplayName = "Hammer")
};

UCLASS()
class TURBOTRACK_API AInteractable : public AActor
{
	GENERATED_BODY()

public:
	AInteractable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	EPickupType PickupType;


	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPickupOverlapSignature);

	UPROPERTY(BlueprintAssignable)
	FPickupOverlapSignature OnSuperNitroPickupOverlap;

	UPROPERTY(BlueprintAssignable)
	FPickupOverlapSignature OnTrashPickupHit;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* HammerSoundCue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* TrashSoundCue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* NitroSoundCue;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnMeshOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                           bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	                    UPrimitiveComponent* OtherComp,
	                    FVector NormalImpulse,
	                    const FHitResult& Hit
	);
	
	UFUNCTION()
	void OnHammerHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	                    UPrimitiveComponent* OtherComp,
	                    FVector NormalImpulse,
	                    const FHitResult& Hit
	);

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;
};

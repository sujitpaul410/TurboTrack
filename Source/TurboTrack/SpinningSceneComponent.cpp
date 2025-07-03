#include "SpinningSceneComponent.h"

USpinningSceneComponent::USpinningSceneComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}



void USpinningSceneComponent::BeginPlay()
{
	Super::BeginPlay();
}



void USpinningSceneComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AddRelativeRotation(RotationSpeed * DeltaTime);
}


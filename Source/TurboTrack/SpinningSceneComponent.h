#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SpinningSceneComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TURBOTRACK_API USpinningSceneComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	USpinningSceneComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spin")
	FRotator RotationSpeed = FRotator(0.f, 90.f, 0.f);

		
};

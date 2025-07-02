#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

UCLASS()
class TURBOTRACK_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	APlayerPawn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnNitro();

	UFUNCTION()
	void OnNitroEnd();

private:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere)
	USceneComponent* FrontLeftWheel;

	UPROPERTY(EditAnywhere)
	USceneComponent* FrontRightWheel;

	UPROPERTY(EditAnywhere)
	USceneComponent* BackLeftWheel;

	UPROPERTY(EditAnywhere)
	USceneComponent* BackRightWheel;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* FrontLeftWheelMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* FrontRightWheelMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BackLeftWheelMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BackRightWheelMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BackWheelBase;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BaseBoxComponent;

	UPROPERTY(EditAnywhere)
	float SpringLength = 120;

	UPROPERTY(EditAnywhere)
	float SpringStiffness = 10000000;

	UPROPERTY(EditAnywhere)
	float AccelerationRate = 1700000;

	UPROPERTY(EditAnywhere)
	float TorqueRate = 500000000;

	UPROPERTY(EditAnywhere)
	float WheelRadius = 66;

	UPROPERTY(EditAnywhere)
	FVector ForwardCenterOfMass = FVector(73, 0, -50);

	UPROPERTY(EditAnywhere)
	FVector RightCenterOfMass = FVector(73, 0, -50);

	UPROPERTY(EditAnywhere)
	FVector LeftCenterOfMass = FVector(73, 0, -50);

	FRotator FrontLeftWheelRotator;

	const int NumWheels = 4;

	void ApplySuspensionAndDrive(USceneComponent* Wheel);

	void Accelerate(USceneComponent* Wheel, bool IsGrounded);

	void Steer(float InputVal);	

	float NitroValue = 1;
	
};

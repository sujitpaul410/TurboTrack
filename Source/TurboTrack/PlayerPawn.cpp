#include "PlayerPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"


APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComponent"));

	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Block);
	BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxComponent->SetSimulatePhysics(true);

	RootComponent = BoxComponent;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarMeshComponent"));
	StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	FrontLeftWheel = CreateDefaultSubobject<USceneComponent>(TEXT("FrontLeftWheel"));
	FrontRightWheel = CreateDefaultSubobject<USceneComponent>(TEXT("FrontRightWheel"));
	BackLeftWheel = CreateDefaultSubobject<USceneComponent>(TEXT("BackLeftWheel"));
	BackRightWheel = CreateDefaultSubobject<USceneComponent>(TEXT("BackRightWheel"));

	FrontLeftWheel->SetupAttachment(RootComponent);
	FrontRightWheel->SetupAttachment(RootComponent);
	BackLeftWheel->SetupAttachment(RootComponent);
	BackRightWheel->SetupAttachment(RootComponent);

	FrontLeftWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontLeftWheelMeshComponent"));
	FrontLeftWheelMesh->AttachToComponent(FrontLeftWheel, FAttachmentTransformRules::KeepRelativeTransform);

	FrontRightWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontRightWheelMeshComponent"));
	FrontRightWheelMesh->AttachToComponent(FrontRightWheel, FAttachmentTransformRules::KeepRelativeTransform);

	BackLeftWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackLeftWheelMeshComponent"));
	BackLeftWheelMesh->AttachToComponent(BackLeftWheel, FAttachmentTransformRules::KeepRelativeTransform);

	BackRightWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackRightWheelMeshComponent"));
	BackRightWheelMesh->AttachToComponent(BackRightWheel, FAttachmentTransformRules::KeepRelativeTransform);

	BackWheelBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackWheelBase"));
	BackWheelBase->AttachToComponent(BackRightWheel, FAttachmentTransformRules::KeepRelativeTransform);
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->SetCenterOfMass(ForwardCenterOfMass);
	FrontLeftWheelRotator = FrontLeftWheel->GetRelativeRotation();
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ApplySuspensionAndDrive(FrontLeftWheel);
	ApplySuspensionAndDrive(FrontRightWheel);
	ApplySuspensionAndDrive(BackLeftWheel);
	ApplySuspensionAndDrive(BackRightWheel);
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Steer", this, &APlayerPawn::Steer);
}

void APlayerPawn::ApplySuspensionAndDrive(USceneComponent* Wheel)
{
	FVector WheelLocation = Wheel->GetComponentLocation();
	FVector EndLocation = WheelLocation + Wheel->GetUpVector() * (-1 * SpringLength);

	FHitResult HitResult;

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		WheelLocation,
		EndLocation,
		ECC_Visibility,
		TraceParams
	);

	if (bHit)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Wheel: %s, Hit distance: %f"), *Wheel->GetName(), HitResult.Distance);

		float NormalizedDistance = FMath::Clamp(1.0f - (HitResult.Distance / SpringLength), 0.0f, 1.0f);

		FVector UnitDir = (HitResult.TraceStart - HitResult.TraceEnd).GetSafeNormal();

		FVector Force = (UnitDir * NormalizedDistance) * SpringStiffness;

		BoxComponent->AddForceAtLocation(Force, WheelLocation);
		// UE_LOG(LogTemp, Warning, TEXT("Force: %s"), *Force.ToString());

		//DrawDebugLine(GetWorld(), WheelLocation, EndLocation, FColor::Red, true, 1.0f, 0, 2.0f);
		// DrawDebugDirectionalArrow(GetWorld(), WheelLocation, WheelLocation+Force, 300, FColor::Green);

		USceneComponent* WheelMesh = Wheel->GetChildComponent(0);
		float WheelZLocation = FMath::FInterpTo(WheelMesh->GetRelativeLocation().Z,
		                                        HitResult.Distance * -1 + WheelRadius, GetWorld()->GetDeltaSeconds(),
		                                        3.0f);
		FVector CurrWheelZLocation = WheelMesh->GetRelativeLocation();
		CurrWheelZLocation.Z = WheelZLocation;
		WheelMesh->SetRelativeLocation(CurrWheelZLocation);
		if (Wheel == BackRightWheel && BackWheelBase)
		{
			FVector CurrWheelBaseLocation = BackWheelBase->GetRelativeLocation();
			CurrWheelBaseLocation.Z = CurrWheelZLocation.Z - WheelRadius;
			BackWheelBase->SetRelativeLocation(CurrWheelBaseLocation);
		}

		Accelerate(Wheel, true);
	}
	else
	{
		// UE_LOG(LogTemp, Warning, TEXT("Nothing hit."));
		Accelerate(Wheel, false);
	}
}

void APlayerPawn::Accelerate(USceneComponent* Wheel, bool bIsGrounded)
{
	FVector Forward = BoxComponent->GetForwardVector();
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	float GroundMultiplier = 0.001;
	if (bIsGrounded)
	{
		GroundMultiplier = 1;
		auto Force = Forward * BoxComponent->GetMass() * AccelerationRate * DeltaTime * GroundMultiplier / NumWheels;
		BoxComponent->AddForce(Force);
	}
	else
	{
		auto Force = Forward * BoxComponent->GetMass() * 5 * DeltaTime * GroundMultiplier / NumWheels;
		BoxComponent->AddForce(Force);
	}

	FRotator Rotator = FRotator(0, 0, 10);
	Wheel->GetChildComponent(0)->AddLocalRotation(Rotator);
}

void APlayerPawn::Steer(float InputVal)
{
	if (InputVal == 0)
	{
		BoxComponent->SetCenterOfMass(ForwardCenterOfMass);
		FrontLeftWheel->SetRelativeRotation(FrontLeftWheelRotator);
		FrontRightWheel->SetRelativeRotation(FrontLeftWheelRotator);
	}
	else if (InputVal == 1)
	{
		BoxComponent->SetCenterOfMass(RightCenterOfMass);

		FRotator CurrentRot = FrontLeftWheel->GetRelativeRotation();
		float NextZRot = FMath::FInterpTo(CurrentRot.Yaw, -60, GetWorld()->GetDeltaSeconds(), 10.0f);
		CurrentRot.Yaw = NextZRot;
		FrontLeftWheel->SetRelativeRotation(CurrentRot);
		FrontRightWheel->SetRelativeRotation(CurrentRot);
	}
	else if (InputVal == -1)
	{
		BoxComponent->SetCenterOfMass(LeftCenterOfMass);

		FRotator CurrentRot = FrontLeftWheel->GetRelativeRotation();
		float NextZRot = FMath::FInterpTo(CurrentRot.Yaw, -120, GetWorld()->GetDeltaSeconds(), 10.0f);
		CurrentRot.Yaw = NextZRot;
		FrontLeftWheel->SetRelativeRotation(CurrentRot);
		FrontRightWheel->SetRelativeRotation(CurrentRot);
	}

	FVector Torque = FVector(0, 0, 0);
	Torque.Z += InputVal * TorqueRate;
	// UE_LOG(LogTemp, Warning, TEXT("InputVal: %f Torque: %ls"), InputVal, *Torque.ToString());
	BoxComponent->AddTorqueInRadians(Torque);
}

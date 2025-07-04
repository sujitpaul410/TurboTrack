#include "StartScreenUserWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


void UStartScreenUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Vehicle")), FoundVehicleActors);
	
	if (NextButton)
	{
		NextButton->OnClicked.AddDynamic(this, &UStartScreenUserWidget::OnNextSelected);
	}
	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &UStartScreenUserWidget::OnStartGameSelected);
	}

	if (FoundVehicleActors.Num() > 1)
	{
		AActor* TargetActor = FoundVehicleActors[1];

		TargetActor->SetActorHiddenInGame(true);
		TargetActor->SetActorEnableCollision(false);
		TargetActor->SetActorTickEnabled(false);
		UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(TargetActor->GetComponentByClass(UPrimitiveComponent::StaticClass()));
		PrimComp->SetSimulatePhysics(false);

		TargetActor = FoundVehicleActors[0];

		TargetActor->SetActorHiddenInGame(false);
		TargetActor->SetActorEnableCollision(true);
		TargetActor->SetActorTickEnabled(true);
		PrimComp = Cast<UPrimitiveComponent>(TargetActor->GetComponentByClass(UPrimitiveComponent::StaticClass()));
		PrimComp->SetSimulatePhysics(true);
	}
}


void UStartScreenUserWidget::OnNextSelected()
{
	UE_LOG(LogTemp, Display, TEXT("OnNextSelected: %d"), CurrIndx);

	if (FoundVehicleActors.Num() == 0)
	{
		return;
	}
	
	if (FoundVehicleActors.Num() > 1)
	{
		int NewIndx = 1 - CurrIndx;
		AActor* TargetActor = FoundVehicleActors[NewIndx];
	
		UE_LOG(LogTemp, Display, TEXT("Target Actor: %s"), *TargetActor->GetName());

		AActor* UsedActor = FoundVehicleActors[CurrIndx];
		TargetActor->SetActorHiddenInGame(false);
		TargetActor->SetActorEnableCollision(true);
		TargetActor->SetActorTickEnabled(true);
		UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(TargetActor->GetComponentByClass(UPrimitiveComponent::StaticClass()));
		PrimComp->SetSimulatePhysics(true);
		
		UsedActor->SetActorHiddenInGame(true);
		UsedActor->SetActorEnableCollision(false);
		UsedActor->SetActorTickEnabled(false);
		PrimComp = Cast<UPrimitiveComponent>(UsedActor->GetComponentByClass(UPrimitiveComponent::StaticClass()));
		PrimComp->SetSimulatePhysics(false);
		UsedActor->SetActorLocation(FVector(0, 0, 523));

		CurrIndx = NewIndx;
	}
}

void UStartScreenUserWidget::OnStartGameSelected()
{
	UE_LOG(LogTemp, Display, TEXT("OnStartGameSelected"));
	if (CurrIndx == 0)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName("TruckDrive"));
	}
	else if (CurrIndx == 1)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName("SedanDrive"));
	}
}

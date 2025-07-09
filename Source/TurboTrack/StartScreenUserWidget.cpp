#include "StartScreenUserWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


class UTextBlock;

void UStartScreenUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Vehicle")), FoundVehicleActors);

	if (NextButton)
	{
		NextButton->OnHovered.AddDynamic(this, &UStartScreenUserWidget::OnButtonHovered);
		NextButton->OnUnhovered.AddDynamic(this, &UStartScreenUserWidget::OnButtonUnHovered);
		NextButton->OnClicked.AddDynamic(this, &UStartScreenUserWidget::OnNextSelected);

		NextBtnTextBlock = Cast<UTextBlock>(NextButton->GetChildAt(0));
		NextBtnInitialColor = NextBtnTextBlock->GetColorAndOpacity();
	}
	if (StartGameButton)
	{
		StartGameButton->OnHovered.AddDynamic(this, &UStartScreenUserWidget::OnButtonHovered);
		StartGameButton->OnUnhovered.AddDynamic(this, &UStartScreenUserWidget::OnButtonUnHovered);
		StartGameButton->OnClicked.AddDynamic(this, &UStartScreenUserWidget::OnStartGameSelected);

		StartBtnTextBlock = Cast<UTextBlock>(StartGameButton->GetChildAt(0));
		StartBtnInitialColor = StartBtnTextBlock->GetColorAndOpacity();
	}

	if (FoundVehicleActors.Num() > 1)
	{
		AActor* TargetActor = FoundVehicleActors[1];

		TargetActor->SetActorHiddenInGame(true);
		TargetActor->SetActorEnableCollision(false);
		TargetActor->SetActorTickEnabled(false);
		UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(
			TargetActor->GetComponentByClass(UPrimitiveComponent::StaticClass()));
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
	// UE_LOG(LogTemp, Display, TEXT("OnNextSelected: %d"), CurrIndx);

	UGameplayStatics::PlaySound2D(this, ClickSoundCue);
	
	if (FoundVehicleActors.Num() == 0)
	{
		return;
	}

	NextButton->SetRenderScale(FVector2D(0.9f, 0.9f));

	if (FoundVehicleActors.Num() > 1)
	{
		int NewIndx = 1 - CurrIndx;
		AActor* TargetActor = FoundVehicleActors[NewIndx];

		// UE_LOG(LogTemp, Display, TEXT("Target Actor: %s"), *TargetActor->GetName());

		AActor* UsedActor = FoundVehicleActors[CurrIndx];
		TargetActor->SetActorHiddenInGame(false);
		TargetActor->SetActorEnableCollision(true);
		TargetActor->SetActorTickEnabled(true);
		UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(
			TargetActor->GetComponentByClass(UPrimitiveComponent::StaticClass()));
		PrimComp->SetSimulatePhysics(true);

		UsedActor->SetActorHiddenInGame(true);
		UsedActor->SetActorEnableCollision(false);
		UsedActor->SetActorTickEnabled(false);
		PrimComp = Cast<UPrimitiveComponent>(UsedActor->GetComponentByClass(UPrimitiveComponent::StaticClass()));
		PrimComp->SetSimulatePhysics(false);
		UsedActor->SetActorLocation(FVector(0, 0, 523));

		CurrIndx = NewIndx;
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	                                       {
		                                       NextButton->SetRenderScale(FVector2D(1.0f, 1.0f));
	                                       },
	                                       0.1f,
	                                       false
	);
}

void UStartScreenUserWidget::OnButtonHovered()
{
	if (NextButton->IsHovered())
	{
		NextBtnTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Blue));
	}
	else if (StartGameButton->IsHovered())
	{
		StartBtnTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Blue));
	}
}

void UStartScreenUserWidget::OnButtonUnHovered()
{
	if (!NextButton->IsHovered())
	{
		NextBtnTextBlock->SetColorAndOpacity(NextBtnInitialColor);
	}
	if (!StartGameButton->IsHovered())
	{
		StartBtnTextBlock->SetColorAndOpacity(StartBtnInitialColor);
	}
}

void UStartScreenUserWidget::OnStartGameSelected()
{
	// UE_LOG(LogTemp, Display, TEXT("OnStartGameSelected"));
	
	UGameplayStatics::PlaySound2D(this, ClickSoundCue);

	AActor* UsedActor = FoundVehicleActors[CurrIndx];

	if (UsedActor->Tags.Contains(FName("Truck")))
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName("TruckDrive"));
	}
	else if (UsedActor->Tags.Contains(FName("Sedan")))
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName("SedanDrive"));
	}
}

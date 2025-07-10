#include "TrackMonitor.h"

#include "MainScreenUserWidget.h"
#include "TrackGenerator.h"
#include "Kismet/GameplayStatics.h"

ATrackMonitor::ATrackMonitor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ATrackMonitor::BeginPlay()
{
	Super::BeginPlay();
	AddMainScreenWidget();
}

void ATrackMonitor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsGameOver && IsPlayerOffTrack())
	{
		UE_LOG(LogTemp, Warning, TEXT("Player off track"));
		OnPlayerOffTrack.Broadcast();
		IsGameOver = true;
	}

}

bool ATrackMonitor::IsPlayerOffTrack()
{
	
	if (!TrackGenerator || !TrackGenerator->SplineComponent)
		return true;

	UWorld* World = TrackGenerator->SplineComponent->GetWorld();
	if (!World) return true;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0);

	FVector PlayerLocation = PlayerPawn->GetActorLocation();

	float InputKey = TrackGenerator->SplineComponent->FindInputKeyClosestToWorldLocation(PlayerLocation);
	FVector ClosestLocation = TrackGenerator->SplineComponent->GetLocationAtSplineInputKey(InputKey, ESplineCoordinateSpace::World);

	float Distance = FVector::Dist(PlayerLocation, ClosestLocation);
	
	return Distance > MaxAllowedDistance;
}

void ATrackMonitor::AddMainScreenWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("AddMainScreenWidget"));
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	if (PC && MainScreenWidget)
	{
		UUserWidget* MainScreenWidgetInst = CreateWidget<UMainScreenUserWidget>(PC, MainScreenWidget);
		if (MainScreenWidgetInst)
		{
			MainScreenWidgetInst->AddToViewport();
		}
		
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(MainScreenWidgetInst->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}
}


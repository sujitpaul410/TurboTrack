#include "TurboPlayerController.h"

#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraActor.h"

void ATurboPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// UE_LOG(LogTemp, Warning, TEXT("TurboPlayerController::BeginPlay"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), CarSelectWidgetClass);
	Widget->AddToViewport();

	for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
	{
		ACameraActor* Cam = *It;

		if (Cam && Cam->ActorHasTag("StartCam"))
		{
			SetViewTargetWithBlend(Cam, 0.1f); // Smooth blend to the camera
			// UE_LOG(LogTemp, Warning, TEXT("TurboPlayerController::SetViewTarget"));
			break;
		}
	}
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	SetInputMode(FInputModeGameAndUI());
}

void ATurboPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ATurboPlayerController::OnClick()
{
}

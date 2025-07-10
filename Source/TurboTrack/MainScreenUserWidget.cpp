#include "MainScreenUserWidget.h"

#include "EngineUtils.h"
#include "TrackMonitor.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UMainScreenUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (RestartBtn)
	{
		RestartBtn->OnHovered.AddDynamic(this, &UMainScreenUserWidget::OnButtonHovered);
		RestartBtn->OnUnhovered.AddDynamic(this, &UMainScreenUserWidget::OnButtonUnHovered);
		RestartBtn->OnClicked.AddDynamic(this, &UMainScreenUserWidget::OnRestartSelected);

		RestartBtnTextBlock = Cast<UTextBlock>(RestartBtn->GetChildAt(0));
		RestartBtnInitialColor = RestartBtnTextBlock->GetColorAndOpacity();

		RestartBtn->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	if (MainScreenBtn)
	{
		MainScreenBtn->OnHovered.AddDynamic(this, &UMainScreenUserWidget::OnButtonHovered);
		MainScreenBtn->OnUnhovered.AddDynamic(this, &UMainScreenUserWidget::OnButtonUnHovered);
		MainScreenBtn->OnClicked.AddDynamic(this, &UMainScreenUserWidget::OnMainSelected);

		MainScreenBtnTextBlock = Cast<UTextBlock>(MainScreenBtn->GetChildAt(0));
		MainScreenBtnInitialColor = MainScreenBtnTextBlock->GetColorAndOpacity();

		MainScreenBtn->SetVisibility(ESlateVisibility::Collapsed);
	}

	ATrackMonitor* TrackMonitorActor = nullptr;

	for (TActorIterator<ATrackMonitor> It(GetWorld()); It; ++It)
	{
		TrackMonitorActor = *It;
		break;
	}

	if (TrackMonitorActor)
	{
		BindToGameOverEvent(TrackMonitorActor);
	}
}



void UMainScreenUserWidget::OnButtonHovered()
{
	if (RestartBtn->IsHovered())
	{
		RestartBtnTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Blue));
	}
	else if (MainScreenBtn->IsHovered())
	{
		MainScreenBtnTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Blue));
	}
}

void UMainScreenUserWidget::OnButtonUnHovered()
{
	if (!RestartBtn->IsHovered())
	{
		RestartBtnTextBlock->SetColorAndOpacity(RestartBtnInitialColor);
	}
	if (!MainScreenBtn->IsHovered())
	{
		MainScreenBtnTextBlock->SetColorAndOpacity(MainScreenBtnInitialColor);
	}
}


void UMainScreenUserWidget::OnRestartSelected()
{
	UGameplayStatics::PlaySound2D(this, ClickSoundCue);
	
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this, true);
	UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName));
}

void UMainScreenUserWidget::OnMainSelected()
{
	UGameplayStatics::PlaySound2D(this, ClickSoundCue);
	
	UGameplayStatics::OpenLevel(GetWorld(), FName("StartLevel"));
}

void UMainScreenUserWidget::OnGameOver()
{
	ShallStopTimer = true;
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->UnPossess();
	}
	MainScreenBtn->SetVisibility(ESlateVisibility::Visible);
	RestartBtn->SetVisibility(ESlateVisibility::Visible);
}

void UMainScreenUserWidget::OnObstacleHit()
{
	FString StringValueHit = *RoadBlockScore->GetText().ToString();
	int IntValueHit = FCString::Atoi(*StringValueHit);

	++IntValueHit;
	
	RoadBlockScore->SetText(FText::AsNumber(IntValueHit));
}

void UMainScreenUserWidget::BindToGameOverEvent(ATrackMonitor* TrackMonitor)
{
	if (TrackMonitor)
	{
		TrackMonitor->OnPlayerOffTrack.AddDynamic(this, &UMainScreenUserWidget::OnGameOver);
	}
}

void UMainScreenUserWidget::StopTimer()
{
	ShallStopTimer = true;
}

bool UMainScreenUserWidget::IsTimerRunning()
{
	return !ShallStopTimer;
}

void UMainScreenUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!ShallStopTimer)
	{
		FString StringValueTime = *TimeScore->GetText().ToString();
		float FloatValueTime = FCString::Atof(*StringValueTime);

		FloatValueTime += InDeltaTime;

		// TimeScore->SetText(FText::AsNumber(FloatValueTime));
		TimeScore->SetText(FText::Format(FText::FromString("{0} s"), FText::AsNumber(FloatValueTime)));
	}
	
}

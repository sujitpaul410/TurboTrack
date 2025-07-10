#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainScreenUserWidget.generated.h"

UCLASS()
class TURBOTRACK_API UMainScreenUserWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* RestartBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* MainScreenBtn;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RoadBlockScore;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeScore;
	
	UTextBlock* RestartBtnTextBlock;
	UTextBlock* MainScreenBtnTextBlock;
	
	FSlateColor RestartBtnInitialColor;
	FSlateColor MainScreenBtnInitialColor;

	bool ShallStopTimer = false;
	void BindToGameOverEvent(class ATrackMonitor* TrackMonitor);

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* ClickSoundCue;

	UFUNCTION()
	void OnButtonHovered();
	
	UFUNCTION()
	void OnButtonUnHovered();

	UFUNCTION()
	void OnRestartSelected();
	
	UFUNCTION()
	void OnMainSelected();

	
	UFUNCTION()
	void OnGameOver();
	
	UFUNCTION()
	void OnObstacleHit();

	void StopTimer();
	bool IsTimerRunning();

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};

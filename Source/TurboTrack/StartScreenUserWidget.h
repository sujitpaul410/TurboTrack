#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartScreenUserWidget.generated.h"


UCLASS()
class TURBOTRACK_API UStartScreenUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	class UButton* NextButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* StartGameButton;
	

	UFUNCTION()
	void OnNextSelected();

	UFUNCTION()
	void OnStartGameSelected();

protected:
	virtual void NativeConstruct() override;

private:
	TArray<AActor*> FoundVehicleActors;
	int CurrIndx = 0;
};

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

	UFUNCTION()
	void OnNextSelected();

protected:
	virtual void NativeConstruct() override;

private:
	TArray<AActor*> FoundVehicleActors;
	int CurrIndx = 0;
};

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h" 

class APlayerController;

#include "TitleWidget.generated.h"

UCLASS()
class ROGUELIKEFPS_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> GameStartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> OptionButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartButtonClicked);
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStartButtonClicked OnStartButtonClicked;

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void OnGameStartClicked();

	UFUNCTION()
	void OnOptionClicked();

	UFUNCTION()
	void OnExitClicked();

};

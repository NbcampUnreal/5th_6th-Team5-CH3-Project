#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameClearWidget.generated.h"
//for git commit
class UButton;
class AFPSGameMode;

UCLASS()
class ROGUELIKEFPS_API UGameClearWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	// GameMode 포인터를 위젯에 전달할 함수
	void SetupGameModeLink(AFPSGameMode* OwningGameMode);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;

private:
	UFUNCTION()
	void OnExitButtonClicked();

	TWeakObjectPtr<class AFPSGameMode> OwningGameModePtr;
};
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameClearWidget.generated.h"

class UButton;
class AFPSGameMode; // GameMode 참조를 위해 필요

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
	TObjectPtr<UButton> ExitButton; // 디자이너 변수명과 일치해야 함

private:
	UFUNCTION()
	void OnExitButtonClicked(); // 함수 이름 통일

	TWeakObjectPtr<class AFPSGameMode> OwningGameModePtr; // 변수 이름 통일
};
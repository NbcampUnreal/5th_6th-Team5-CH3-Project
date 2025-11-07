#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h" 
#include "TitleWidget.generated.h"

class UButton;
class UMainMenuWidget;

UCLASS()
class ROGUELIKEFPS_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> OptionButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;

	// 블루프린트에서 MainMenu 위젯 클래스를 지정할 수 있게 추가 (AFPSGameMode에서 사용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

	// 블루프린트에서 다른 위젯이 Start 버튼 클릭 이벤트를 구독할 수 있도록
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartButtonClicked);
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStartButtonClicked OnStartButtonClicked;

protected:

private:
	UFUNCTION()
	void OnGameStartClicked();

	UFUNCTION()
	void OnOptionClicked();

	UFUNCTION()
	void OnExitClicked();

};
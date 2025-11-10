#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "TitleWidget.generated.h"
//for git commit
class UMainMenuWidget;

UCLASS()
class ROGUELIKEFPS_API UTitleWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    virtual void NativeConstruct() override;
    virtual bool Initialize() override;

    // UI 버튼 바인딩
    UPROPERTY(meta = (BindWidgetOptional)) // Optional로 null 체크 간소화
    TObjectPtr<UButton> StartButton; // 시작 버튼
    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UButton> OptionButton; // 옵션 버튼
    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UButton> ExitButton; // 종료 버튼

    // 메인 메뉴 위젯 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;
	
    // 시작 버튼 이벤트 델리게이트
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartButtonClicked);
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnStartButtonClicked OnStartButtonClicked;

private:
    UFUNCTION()
    void OnGameStartClicked(); // 시작 버튼 클릭
    UFUNCTION()
    void OnOptionClicked(); // 옵션 버튼 클릭
    UFUNCTION()
    void OnExitClicked(); // 종료 버튼 클릭
};

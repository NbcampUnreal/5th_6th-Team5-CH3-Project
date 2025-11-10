#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "DeathWidget.generated.h"
//for git commit
class APlayerController;

UCLASS()
class ROGUELIKEFPS_API UDeathWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    // 초기 설정 (사망/클리어 여부 지정)
    UFUNCTION(BlueprintCallable)
    void Setup(APlayerController* InController, bool bInIsCleared);

protected:
    virtual bool Initialize() override;

    // UI 버튼 바인딩
    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UButton> RestartButton; // 재시작 버튼
    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UButton> ExitButton; // 메인 메뉴로 버튼

    // 클리어 여부 플래그
    UPROPERTY(BlueprintReadWrite)
    bool bIsGameCleared = false;

private:
    // 소유 컨트롤러
    TWeakObjectPtr<APlayerController> OwningController;
    UFUNCTION()
    void OnRestartClicked(); // 재시작 처리
    UFUNCTION()
    void OnExitClicked(); // 메인 메뉴로 이동
};
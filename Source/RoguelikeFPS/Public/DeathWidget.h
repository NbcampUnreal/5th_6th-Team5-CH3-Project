#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

class UButton;
class APlayerController;

#include "DeathWidget.generated.h"

//Death라고 했지만 사실 Clear도 포함
UCLASS()
class ROGUELIKEFPS_API UDeathWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    // UI 버튼을 UMG 디자이너에서 변수 이름으로 연결하기 위한 BindWidget
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> RestartButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> QuitButton;

    // 이 위젯을 소유하는 컨트롤러 포인터 (클릭 이벤트 처리용)
    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<APlayerController> OwningController;

protected:
    virtual bool Initialize() override;

private:
    UFUNCTION()
    void OnRestartClicked();

    UFUNCTION()
    void OnQuitClicked();
};
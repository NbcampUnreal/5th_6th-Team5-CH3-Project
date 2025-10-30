#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuController.generated.h"

class UTitleWidget;
class UMainMenuWidget;
class UUserWidget;

UCLASS()
class ROGUELIKEFPS_API AMainMenuController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// 에디터에서 MainMenuWidget 블루프린트를 할당받을 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UTitleWidget> TitleWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

private:
	// 생성된 위젯 인스턴스를 저장
	UPROPERTY()
	TObjectPtr<UTitleWidget> TitleWidgetInstance;

	UPROPERTY()
	TObjectPtr<UMainMenuWidget> MainMenuWidgetInstance;

	UFUNCTION()
	void ShowWeaponSelectMenu();
};

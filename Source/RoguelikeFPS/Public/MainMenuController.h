#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h" 
#include "MainMenuController.generated.h"

UCLASS()
class ROGUELIKEFPS_API AMainMenuController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// 에디터에서 MainMenuWidget 블루프린트를 할당받을 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

private:
	// 생성된 위젯 인스턴스를 저장
	UPROPERTY()
	UUserWidget* MainMenuWidgetInstance;
};

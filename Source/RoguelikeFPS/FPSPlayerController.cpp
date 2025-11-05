#include "FPSPlayerController.h"
#include "EnhancedInputSubsystems.h"
//#include "Blueprint/UserWidget.h"
//#include "TitleWidget.h"

AFPSPlayerController::AFPSPlayerController()
	: InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	JumpAction(nullptr),
	CrouchAction(nullptr),
	DashAction(nullptr),
	Weapon_FireAction(nullptr),
	Weapon_Fire_AutoAction(nullptr),
	Weapon_ReloadAction(nullptr)
	//,TitleWidgetClass(nullptr) // Title 추가

{
}

// IMC 활성화
void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (TObjectPtr<ULocalPlayer> LocalPlayer = GetLocalPlayer())
	{
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() )
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
	}

	////Title 추가
	//if (TitleWidgetClass)
	//{
	//	UUserWidget* TitleWidget = CreateWidget<UUserWidget>(this, TitleWidgetClass);
	//	if (TitleWidget)
	//	{
	//		TitleWidget->AddToViewport();

	//		// 마우스 커서 보이기
	//		bShowMouseCursor = true;

	//		FInputModeUIOnly InputMode;
	//		SetInputMode(InputMode);

	//		UE_LOG(LogTemp, Warning, TEXT("Title Widget displayed successfully from PlayerController."));
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Error, TEXT("TitleWidgetClass exists but failed to create widget."));
	//	}
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("TitleWidgetClass not set in PlayerController!"));
	//}

}

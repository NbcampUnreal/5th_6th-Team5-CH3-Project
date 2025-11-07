#include "FPSPlayerController.h"
#include "FPSCharacter.h"
#include "Inventory.h"
#include "InventoryWidget.h"
#include "Shop.h"
#include "RoguelikeFPS/InventoryWidget/ShopWidget.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
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
	Weapon_ReloadAction(nullptr),
	IA_InventoryOnOff(nullptr),
	InventoryWidget(nullptr),
	ShopWidget(nullptr)
	//,TitleWidgetClass(nullptr) // Title 추가

{
}

// IMC 활성화
void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (TObjectPtr<ULocalPlayer> LocalPlayer = GetLocalPlayer())
	{
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
	}

	if (InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(this, InventoryWidgetClass);
		if (InventoryWidget)
		{
			if (AFPSCharacter* MyPlayer = Cast<AFPSCharacter>(GetPawn()))
			{
				InventoryWidget->InitInventory(MyPlayer->Inventory);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("AFPSPlayerController:: MyPlayer is nullptr!"));
			}
			InventoryWidget->AddToViewport();
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	if (ShopWidgetClass)
	{
		ShopWidget = CreateWidget<UShopWidget>(this, ShopWidgetClass);
		if (ShopWidget)
		{
			ShopWidget->AddToViewport();
			ShopWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	SetUpInputBinding();
}
	////title 추가
	//if (titlewidgetclass)
	//{
	//	uuserwidget* titlewidget = createwidget<uuserwidget>(this, titlewidgetclass);
	//	if (titlewidget)
	//	{
	//		titlewidget->addtoviewport();

	//		// 마우스 커서 보이기
	//		bshowmousecursor = true;

	//		finputmodeuionly inputmode;
	//		setinputmode(inputmode);

	//		ue_log(logtemp, warning, text("title widget displayed successfully from playercontroller."));
	//	}
	//	else
	//	{
	//		ue_log(logtemp, error, text("titlewidgetclass exists but failed to create widget."));
	//	}
	//}
	//else
	//{
	//	ue_log(logtemp, error, text("titlewidgetclass not set in playercontroller!"));
	//}

void AFPSPlayerController::SetUpInputBinding()
{
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (IA_InventoryOnOff)
		{
			EnhancedInput->BindAction(
				IA_InventoryOnOff,
				ETriggerEvent::Triggered,
				this,
				&AFPSPlayerController::InventoryToggle
			);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Inventory InputAction is nullptr!"));
		}
	}
}
void AFPSPlayerController::InventoryToggle()
{
	if (!InventoryWidget)
	{
		return;
	}
	if (InventoryWidget->IsVisible())
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI().SetWidgetToFocus(InventoryWidget->TakeWidget()));
	}
}

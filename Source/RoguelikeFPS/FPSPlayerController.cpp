#include "FPSPlayerController.h"
#include "FPSCharacter.h"
#include "Inventory.h"
#include "InventoryWidget.h"
#include "UpgradeMenuWidget.h"
#include "UpgradeSystem.h"
#include "CraftingSystem.h"
#include "CraftingMenuWidget.h"
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

	if (UpgradeMenuWidgetClass)
	{
		UpgradeMenuWidget = CreateWidget<UUpgradeMenuWidget>(this, UpgradeMenuWidgetClass);
		if (UpgradeMenuWidget)
		{
			UpgradeMenuWidget->AddToViewport();
			UpgradeMenuWidget->SetVisibility(ESlateVisibility::Hidden);

			if (APawn* PlayerPawn = GetPawn())
			{
				UInventory* Inv = PlayerPawn->FindComponentByClass<UInventory>();
				UUpgradeSystem* UpSys = PlayerPawn->FindComponentByClass<UUpgradeSystem>();
				UpgradeMenuWidget->InitMenu(Inv, UpSys);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UpgradeMenuWidgetClass is not set in PlayerController!"));
	}

	if (CraftingMenuWidgetClass)
	{
		CraftingMenuWidget = CreateWidget<UCraftingMenuWidget>(this, CraftingMenuWidgetClass);
		if (CraftingMenuWidget)
		{
			CraftingMenuWidget->AddToViewport();
			CraftingMenuWidget->SetVisibility(ESlateVisibility::Hidden);

			if (APawn* MyPawn = GetPawn())
			{
				UInventory* Inven = MyPawn->FindComponentByClass<UInventory>();
				UCraftingSystem* CraftSys = NewObject<UCraftingSystem>(this);
				CraftingMenuWidget->InitMenu(Inven, CraftSys, Inven->ItemDataTable);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CraftingMenuWidgetClass is not set in PlayerController!"));
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
		if (IA_UpgradeOnOff)
		{
			EnhancedInput->BindAction(
				IA_UpgradeOnOff,
				ETriggerEvent::Triggered,
				this,
				&AFPSPlayerController::UpgradeToggle
			);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Upgrade InputAction is nullptr!"));
		}
		if (IA_CraftingOnOff)
		{
			EnhancedInput->BindAction(
				IA_CraftingOnOff,
				ETriggerEvent::Triggered,
				this,
				&AFPSPlayerController::CraftingToggle
			);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Upgrade InputAction is nullptr!"));
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
void AFPSPlayerController::UpgradeToggle()
{
	if (!UpgradeMenuWidget)
	{
		return;
	}

	if (UpgradeMenuWidget->IsVisible())
	{
		UpgradeMenuWidget->SetVisibility(ESlateVisibility::Hidden);
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		UpgradeMenuWidget->SetVisibility(ESlateVisibility::Visible);
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI().SetWidgetToFocus(UpgradeMenuWidget->TakeWidget()));
	}
}
void AFPSPlayerController::CraftingToggle()
{
	if (!CraftingMenuWidget)
		return;

	if (CraftingMenuWidget->IsVisible())
	{
		CraftingMenuWidget->SetVisibility(ESlateVisibility::Hidden);
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		CraftingMenuWidget->SetVisibility(ESlateVisibility::Visible);
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI().SetWidgetToFocus(CraftingMenuWidget->TakeWidget()));
	}
}
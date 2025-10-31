#include "MyPlayerController.h"
#include "MyCharacter.h"
#include "ItemBase.h"
#include "RoguelikeFPS/InventoryWidget/ShopItemWidget.h"
#include "RoguelikeFPS/InventoryWidget/ShopWidget.h"
#include "InventoryWidget.h"
#include "Inventory.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!InventoryWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryWidgetClass is not set in PlayerController"));
		return;
	}

	InventoryWidget = CreateWidget<UInventoryWidget>(this, InventoryWidgetClass);
	if (!InventoryWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create InventoryWidget"));
		return;
	}

	if (!PlayerInventory)
	{
		if (AMyCharacter* MyChar = Cast<AMyCharacter>(GetPawn()))
		{
			if (MyChar->Inventory)
			{
				PlayerInventory = MyChar->Inventory;
			}
		}

		if (!PlayerInventory)
		{
			PlayerInventory = NewObject<UInventory>(this, UInventory::StaticClass());
			PlayerInventory->RegisterComponent();
		}
	}

	InventoryWidget->InitInventory(PlayerInventory);
	InventoryWidget->AddToViewport();
	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);

	if (ShopWidgetClass)
	{
		ShopWidget = CreateWidget<UShopWidget>(this, ShopWidgetClass);
		if (ShopWidget)
		{
			ShopWidget->AddToViewport();
			ShopWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (IMC_Inventory)
				Subsystem->AddMappingContext(IMC_Inventory, 0);
		}
	}
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (IA_InventoryOnOff)
		{
			EnhancedInput->BindAction(IA_InventoryOnOff, ETriggerEvent::Triggered, this, &AMyPlayerController::InventoryOnOff);
		}
	}
}

void AMyPlayerController::InventoryOnOff()
{
	if (!InventoryWidget)
	{
		return;
	}
	bool bVisible = InventoryWidget->IsVisible();
	InventoryWidget->SetVisibility(bVisible ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	if (!bVisible)
	{
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI().SetWidgetToFocus(InventoryWidget->TakeWidget()));
	}
	else
	{
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
}

void AMyPlayerController::OpenShop(const TArray<UItemBase*>& ShopItems)
{
	if (!ShopWidget)
	{
		return;
	}
	ShopWidget->SetVisibility(ESlateVisibility::Visible);
	ShopWidget->InitShop(ShopItems);

	bShowMouseCursor = true;
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(ShopWidget->TakeWidget());
	SetInputMode(InputMode);
}

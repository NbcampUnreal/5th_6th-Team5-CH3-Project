#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ItemBase.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerInventory = NewObject<UInventory>(this);

	if (InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(this, InventoryWidgetClass);
		if (InventoryWidget)
		{
			InventoryWidget->AddToViewport();
			InventoryWidget->InitInventory(PlayerInventory);
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
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
	if (!InventoryWidget) return;

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

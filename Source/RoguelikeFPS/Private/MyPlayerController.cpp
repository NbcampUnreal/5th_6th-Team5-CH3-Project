#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerInventory = NewObject<UInventory>(this);
	PlayerInventory->AddToRoot();

	UItemBase* SampleItem = NewObject<UItemBase>();
	SampleItem->ItemName = "Potion";
	SampleItem->Amount = 3;
	PlayerInventory->AddItem(SampleItem);

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

	InputComponent->BindAction("InventoryOnOff", IE_Pressed, this, &AMyPlayerController::InventoryOnOff);
}

void AMyPlayerController::InventoryOnOff()
{
	if (!InventoryWidget)
	{
		return;
	}
	if (InventoryWidget->IsVisible())
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

#include "InventoryWidget.h"
#include "Inventory.h"
#include "RoguelikeFPS/InventoryWidget/InventorySlotWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void UInventoryWidget::InitInventory(UInventory* InInventory)
{
	Inventory = InInventory;
	UpdateUI();

}
void UInventoryWidget::UpdateUI()
{
	if (!Inventory || !ItemBox || !GoldText)
	{
		return;
	}

	GoldText->SetText(FText::FromString(FString::Printf(TEXT("Gold : %d"), Inventory->Gold)));
	ItemBox->ClearChildren();

	for (UItemBase* Item : Inventory->InventoryItems)
	{
		if (!Item)
		{
			continue;
		}
		UInventorySlotWidget* ItemWidget = CreateWidget < UInventorySlotWidget > (this, InventorySlotWidgetClass);
		if (ItemWidget)
		{
			ItemWidget->SetItemData(Item);
			ItemBox->AddChild(ItemWidget);
		}
	}
}
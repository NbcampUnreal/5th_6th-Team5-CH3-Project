#include "InventoryWidget.h"
#include "Inventory.h"
#include "RoguelikeFPS/InventoryWidget/InventoryItemWidget.h"
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
		UInventoryItemWidget* ItemWidget = CreateWidget<UInventoryItemWidget>(this, InventoryItemWidgetClass);
		if (ItemWidget)
		{
			ItemWidget->SetItemData(Item);
			ItemBox->AddChild(ItemWidget);
		}
	}
}
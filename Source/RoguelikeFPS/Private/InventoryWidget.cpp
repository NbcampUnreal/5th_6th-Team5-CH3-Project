#include "InventoryWidget.h"

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

	for (UItemBase* Item : Inventory->Items)
	{
		if (Item)
		{
			UTextBlock* ItemText = NewObject<UTextBlock>(this);
			ItemText->SetText(FText::FromString(Item->ItemName.ToString() + " x" + FString::FromInt(Item->Amount)));
			ItemBox->AddChild(ItemText);
		}
	}
}

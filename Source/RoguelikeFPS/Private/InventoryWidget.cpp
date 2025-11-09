#include "InventoryWidget.h"
#include "Inventory.h"
#include "RoguelikeFPS/InventoryWidget/InventorySlotWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/TextBlock.h"

void UInventoryWidget::InitInventory(UInventory* InInventory)
{
	Inventory = InInventory;
	
	if (Inventory)
	{
		Inventory->OnGoldChanged.AddDynamic(this, &UInventoryWidget::UpdateGoldUI);
        Inventory->OnInventoryUpdated.AddDynamic(this, &UInventoryWidget::UpdateUI);
	}

	UpdateUI();

}
void UInventoryWidget::UpdateUI()
{
    if (!Inventory || !ItemGrid || !InventorySlotWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("InventoryWidget: Missing references"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("InventoryWidget::UpdateUI() using %s (Items: %d)"),
        *GetNameSafe(Inventory), Inventory->InventoryItems.Num());

    ItemGrid->ClearChildren();

    int32 Row = 0;
    int32 Col = 0;

    for (UItemBase* Item : Inventory->InventoryItems)
    {
        if (!Item) continue;

        UInventorySlotWidget* ItemWidget = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);
        if (!ItemWidget) continue;

        ItemWidget->SetItemData(Item);
        ItemWidget->OnSlotClicked.AddDynamic(this, &UInventoryWidget::HandleSlotClicked);

        if (UUniformGridSlot* GridSlot = ItemGrid->AddChildToUniformGrid(ItemWidget, Row, Col))
        {
            GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
            GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
        }

        Col++;
        if (Col >= Columns)
        {
            Col = 0;
            Row++;
        }
    }

    const int32 MaxSlots = 50;
    const int32 CurrentCount = Inventory->InventoryItems.Num();
    for (int32 i = CurrentCount; i < MaxSlots; ++i)
    {
        UInventorySlotWidget* EmptyWidget = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);
        if (!EmptyWidget) continue;

        EmptyWidget->SetEmptySlot();
        if (UUniformGridSlot* GridSlot = ItemGrid->AddChildToUniformGrid(EmptyWidget, Row, Col))
        {
            GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
            GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
        }

        Col++;
        if (Col >= Columns)
        {
            Col = 0;
            Row++;
        }
    }

    InvalidateLayoutAndVolatility();
}

void UInventoryWidget::UpdateGoldUI(int32 NewGold)
{
	if (GoldText)
	{
		GoldText->SetText(FText::FromString(FString::Printf(TEXT("Gold : %d"), NewGold)));
	}
}
void UInventoryWidget::HandleSlotClicked(UItemBase* ClickedItem)
{
	OnItemClicked.Broadcast(ClickedItem);
}
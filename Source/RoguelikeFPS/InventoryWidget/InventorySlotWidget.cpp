#include "InventoryWidget/InventorySlotWidget.h"
#include "ItemBase.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UInventorySlotWidget::SetItemData(UItemBase* InItem)
{
    ItemData = InItem;
    if (ItemData && ItemName && ItemAmount)
    {
        ItemName->SetText(FText::FromName(ItemData->ItemName));
        ItemAmount->SetText(FText::AsNumber(ItemData->Amount));
        ItemImage->SetBrushFromTexture(ItemData->Thumbnail);
    }
}
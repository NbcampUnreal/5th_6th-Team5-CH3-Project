#include "InventoryWidget/InventorySlotWidget.h"
#include "ItemBase.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SlotButton)
	{
		SlotButton->OnClicked.Clear();
		SlotButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnSlotClickedInternal);
	}
}

void UInventorySlotWidget::SetItemData(UItemBase* InItem)
{
	ItemData = InItem;

	if (ItemData)
	{
		if (ItemAmount)
		{
			ItemAmount->SetText(FText::AsNumber(ItemData->Amount));
		}
		if (ItemImage && ItemData->Thumbnail)
		{
			ItemImage->SetBrushFromTexture(ItemData->Thumbnail);
		}
		else if (ItemImage)
		{
			ItemImage->SetBrushFromTexture(nullptr);
		}
	}

	if (SlotButton)
	{
		SlotButton->OnClicked.Clear();
		SlotButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnSlotClickedInternal);
	}
}

void UInventorySlotWidget::SetEmptySlot()
{
	ItemData = nullptr;

	if (ItemAmount)
	{
		ItemAmount->SetText(FText::FromString(TEXT("")));
	}
	if (ItemImage)
	{
		ItemImage->SetBrushFromTexture(EmptySlotTexture);
	}
	ItemDescription = TEXT("Empty Slot");
	SetToolTipText(FText::FromString(ItemDescription));
}

void UInventorySlotWidget::OnSlotClickedInternal()
{
	if (ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[InventorySlotWidget] Slot clicked: %s"), *ItemData->ItemName.ToString());
		OnSlotClicked.Broadcast(ItemData);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[InventorySlotWidget] Empty slot clicked"));
	}
}
#include "PartMenuWidget.h"
#include "InventoryWidget.h"
#include "PartWidget.h"
#include "Inventory.h"
#include "PartSystem.h"
#include "ItemBase.h"

void UPartMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (InventoryWidget)
    {
        InventoryWidget->OnItemClicked.AddDynamic(this, &UPartMenuWidget::OnInventoryItemClicked);
    }
}

void UPartMenuWidget::InitMenu(UInventory* InInventory, UPartSystem* InPartSystem)
{
    InventoryRef = InInventory;
    PartSystemRef = InPartSystem;

    if (InventoryWidget && InInventory)
    {
        InventoryWidget->InitInventory(InInventory);
    }

    if (PartWidget && InPartSystem)
    {
        PartWidget->InitWidget(InPartSystem, InInventory);
    }
}

void UPartMenuWidget::OnInventoryItemClicked(UItemBase* ClickedItem)
{
    if (!ClickedItem || !PartSystemRef)
    {
        return;
    }

    if (ClickedItem->ItemType == EItemType::PartItem)
    {
        PartSystemRef->SetPendingPart(ClickedItem);
        UE_LOG(LogTemp, Warning, TEXT("[PartMenuWidget] Selected part for equip: %s"), *ClickedItem->ItemName.ToString());
    }
}

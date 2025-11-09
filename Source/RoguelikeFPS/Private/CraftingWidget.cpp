#include "CraftingWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "CraftingSystem.h"
#include "Inventory.h"
#include "ItemData.h"
#include "Engine/DataTable.h"

void UCraftingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ActionButton)
    {
        ActionButton->OnClicked.AddDynamic(this, &UCraftingWidget::OnCraftOrDecomposeClicked);
    }

    UpdateUI();
}

void UCraftingWidget::InitWidget(UCraftingSystem* InCraftingSystem, UInventory* InInventory, UDataTable* InItemDataTable)
{
    CraftingSystem = InCraftingSystem;
    Inventory = InInventory;
    ItemDataTable = InItemDataTable;

    UpdateUI();
}

void UCraftingWidget::SetCraftingMode(ECraftingMode NewMode)
{
    CurrentMode = NewMode;
    UpdateUI();
}

void UCraftingWidget::UpdateUI()
{
    if (!ItemDataTable)
        return;

    const FString Context(TEXT("UpdateUI"));
    FItemData* ItemInfo = ItemDataTable->FindRow<FItemData>(SelectedItemName, Context);

    if (!ItemInfo)
    {
        if (ItemName)
            ItemName->SetText(FText::FromString(TEXT("No Item Selected")));
        if (MaterialListText)
            MaterialListText->SetText(FText::FromString(TEXT("-")));
        if (ActionButtonText)
            ActionButtonText->SetText(FText::FromString(TEXT("Select Item")));
        return;
    }

    if (ItemName)
        ItemName->SetText(FText::FromName(ItemInfo->ItemName));

    FString MaterialInfo;

    if (CurrentMode == ECraftingMode::Craft)
    {
        for (auto& Pair : ItemInfo->CraftingItems)
            MaterialInfo += FString::Printf(TEXT("• %s x%d\n"), *Pair.Key.ToString(), Pair.Value);

        if (ActionButtonText)
            ActionButtonText->SetText(FText::FromString(TEXT("Craft Item")));
    }
    else
    {
        for (auto& Pair : ItemInfo->DestroyItems)
            MaterialInfo += FString::Printf(TEXT("• %s x%d\n"), *Pair.Key.ToString(), Pair.Value);

        if (ActionButtonText)
            ActionButtonText->SetText(FText::FromString(TEXT("Decompose Item")));
    }

    if (MaterialListText)
        MaterialListText->SetText(FText::FromString(MaterialInfo));

    if (CraftStatusText)
        CraftStatusText->SetText(FText::GetEmpty());
}

void UCraftingWidget::OnCraftOrDecomposeClicked()
{
    if (!CraftingSystem || !Inventory || !ItemDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("[CraftingWidget] Missing references"));
        return;
    }

    bool bSuccess = false;

    if (CurrentMode == ECraftingMode::Craft)
        bSuccess = CraftingSystem->CraftItem(Inventory, ItemDataTable, SelectedItemName);
    else
        bSuccess = CraftingSystem->DecomposeItem(Inventory, ItemDataTable, SelectedItemName);

    if (CraftStatusText)
    {
        CraftStatusText->SetText(
            FText::FromString(bSuccess ? TEXT("Success!") : TEXT("Failed: Missing Materials"))
        );
    }

    if (bSuccess)
        OnCraftingCompleted.Broadcast();

    UpdateUI();
}
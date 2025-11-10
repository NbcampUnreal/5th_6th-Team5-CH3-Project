#include "CraftingSystem.h"
#include "Inventory.h"
#include "ItemBase.h"
#include "Engine/DataTable.h"
#include "ItemData.h"

int32 UCraftingSystem::GetItemCount(UInventory* PlayerInventory, const FName& ItemName) const
{
    if (!PlayerInventory) return 0;

    int32 Count = 0;
    for (const UItemBase* Item : PlayerInventory->InventoryItems)
    {
        if (Item && Item->ItemName == ItemName)
            Count += Item->Amount;
    }
    return Count;
}

bool UCraftingSystem::RemoveItem(UInventory* PlayerInventory, const FName& ItemName, int32 Amount)
{
    if (!PlayerInventory || Amount <= 0)
        return false;

    for (int32 i = 0; i < PlayerInventory->InventoryItems.Num(); i++)
    {
        UItemBase* Item = PlayerInventory->InventoryItems[i];
        if (Item && Item->ItemName == ItemName)
        {
            const int32 RemoveCount = FMath::Min(Item->Amount, Amount);
            Item->Amount -= RemoveCount;
            Amount -= RemoveCount;

            if (Item->Amount <= 0)
                PlayerInventory->InventoryItems.RemoveAt(i--);

            if (Amount <= 0)
                return true;
        }
    }
    return false;
}

bool UCraftingSystem::AddItem(UInventory* PlayerInventory, UDataTable* ItemDataTable, const FName& ItemName, int32 Amount)
{
    if (!PlayerInventory || !ItemDataTable || ItemName == NAME_None || Amount <= 0)
    {
        return false;
    }
    const FString Context(TEXT("AddItemFromCraft"));
    FItemData* ItemInfo = ItemDataTable->FindRow<FItemData>(ItemName, Context);
    if (!ItemInfo)
    {
        UE_LOG(LogTemp, Warning, TEXT("[CraftingSystem] Cannot find item row: %s"), *ItemName.ToString());
        return false;
    }

    UItemBase* ExistingItem = PlayerInventory->SearchItemName(ItemName);
    if (ExistingItem)
    {
        ExistingItem->Amount += Amount;
        PlayerInventory->OnInventoryUpdated.Broadcast();
        return true;
    }

    UItemBase* NewItem = NewObject<UItemBase>(PlayerInventory);
    if (NewItem)
    {
        NewItem->InitItemData(*ItemInfo);
        NewItem->Amount = Amount;
        PlayerInventory->InventoryItems.Add(NewItem);

        PlayerInventory->OnInventoryUpdated.Broadcast();
        UE_LOG(LogTemp, Log, TEXT("[CraftingSystem] Created new crafted item: %s (x%d)"), *ItemName.ToString(), Amount);
        return true;
    }

    return false;
}

// 제작

///////////////////////////////////////////////////////////

bool UCraftingSystem::CraftItem(UInventory* PlayerInventory, UDataTable* ItemDataTable, const FName& TargetItemName)
{
    if (!PlayerInventory || !ItemDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("[CraftingSystem] Invalid references."));
        return false;
    }

    const FString Context(TEXT("Crafting"));
    FItemData* ItemInfo = ItemDataTable->FindRow<FItemData>(TargetItemName, Context);

    if (!ItemInfo)
    {
        UE_LOG(LogTemp, Warning, TEXT("[CraftingSystem] No data found for %s"), *TargetItemName.ToString());
        return false;
    }

    if (ItemInfo->CraftingItems.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("[CraftingSystem] %s has no crafting recipe."), *TargetItemName.ToString());
        return false;
    }

    for (const auto& Pair : ItemInfo->CraftingItems)
    {
        int32 Have = GetItemCount(PlayerInventory, Pair.Key);
        if (Have < Pair.Value)
        {
            UE_LOG(LogTemp, Warning, TEXT("[CraftingSystem] Not enough %s (Need %d, Have %d)"),
                *Pair.Key.ToString(), Pair.Value, Have);
            return false;
        }
    }

    for (const auto& Pair : ItemInfo->CraftingItems)
    {
        RemoveItem(PlayerInventory, Pair.Key, Pair.Value);
    }

    AddItem(PlayerInventory, ItemDataTable, TargetItemName, 1);

    UE_LOG(LogTemp, Log, TEXT("[CraftingSystem] Crafted new item: %s"), *TargetItemName.ToString());
    return true;
}
// 분해

///////////////////////////////////////////////////////////
// 분해
///////////////////////////////////////////////////////////

bool UCraftingSystem::DecomposeItem(UInventory* PlayerInventory, UDataTable* ItemDataTable, const FName& TargetItemName)
{
    if (!PlayerInventory || !ItemDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("[CraftingSystem] Invalid references."));
        return false;
    }

    const FString Context(TEXT("Decompose"));
    FItemData* ItemInfo = ItemDataTable->FindRow<FItemData>(TargetItemName, Context);

    if (!ItemInfo)
    {
        UE_LOG(LogTemp, Warning, TEXT("[CraftingSystem] No data found for %s"), *TargetItemName.ToString());
        return false;
    }

    if (ItemInfo->DestroyItems.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("[CraftingSystem] %s cannot be decomposed."), *TargetItemName.ToString());
        return false;
    }

    int32 Count = GetItemCount(PlayerInventory, TargetItemName);
    if (Count <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("[CraftingSystem] You don't have %s to decompose."), *TargetItemName.ToString());
        return false;
    }

    RemoveItem(PlayerInventory, TargetItemName, 1);

    for (const auto& Pair : ItemInfo->DestroyItems)
    {
        AddItem(PlayerInventory, ItemDataTable, Pair.Key, Pair.Value);
        UE_LOG(LogTemp, Log, TEXT("[CraftingSystem] Decomposed %s -> %s x%d"),
            *TargetItemName.ToString(), *Pair.Key.ToString(), Pair.Value);
    }

    return true;
}
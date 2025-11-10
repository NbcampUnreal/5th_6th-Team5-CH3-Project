#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CraftingSystem.generated.h"

class UInventory;
class UItemBase;
class UDataTable;

UCLASS(Blueprintable)
class ROGUELIKEFPS_API UCraftingSystem : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    bool CraftItem(UInventory* PlayerInventory, UDataTable* ItemDataTable, const FName& TargetItemName);

    UFUNCTION(BlueprintCallable)
    bool DecomposeItem(UInventory* PlayerInventory, UDataTable* ItemDataTable, const FName& TargetItemName);

private:
    int32 GetItemCount(UInventory* PlayerInventory, const FName& ItemName) const;

    bool RemoveItem(UInventory* PlayerInventory, const FName& ItemName, int32 Amount);

    bool AddItem(UInventory* PlayerInventory, UDataTable* ItemDataTable, const FName& ItemName, int32 Amount);
};
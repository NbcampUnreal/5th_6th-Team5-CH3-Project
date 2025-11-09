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
    /** 아이템 제작 */
    UFUNCTION(BlueprintCallable)
    bool CraftItem(UInventory* PlayerInventory, UDataTable* ItemDataTable, const FName& TargetItemName);

    /** 아이템 분해 */
    UFUNCTION(BlueprintCallable)
    bool DecomposeItem(UInventory* PlayerInventory, UDataTable* ItemDataTable, const FName& TargetItemName);

private:
    /** 내부 유틸: 인벤토리에서 해당 아이템의 총 개수 가져오기 */
    int32 GetItemCount(UInventory* PlayerInventory, const FName& ItemName) const;

    /** 내부 유틸: 인벤토리에서 해당 아이템 제거 */
    bool RemoveItem(UInventory* PlayerInventory, const FName& ItemName, int32 Amount);

    /** 내부 유틸: 인벤토리에 새 아이템 추가 */
    bool AddItem(UInventory* PlayerInventory, UDataTable* ItemDataTable, const FName& ItemName, int32 Amount);
};
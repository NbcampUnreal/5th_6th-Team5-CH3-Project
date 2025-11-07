#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CraftingSystem.generated.h"

class UPartItem;
class UResourceItem;
class UDataTable;
struct FPartData;

UCLASS(Blueprintable)
class ROGUELIKEFPS_API UCraftingSystem : public UObject
{
    GENERATED_BODY()
public:

    UFUNCTION(BlueprintCallable)
    void DecomposePart(UPartItem* Part, TArray<UResourceItem*>& PlayerResources);

    UFUNCTION(BlueprintCallable)
    bool CraftPart(
        TArray<UPartItem*>& PlayerParts,
        TArray<UResourceItem*>& PlayerResources,
        const TMap<TSubclassOf<UResourceItem>, int32>& RequiredResources,
        const TMap<TSubclassOf<UPartItem>, int32>& RequiredParts,
        UDataTable* PartDataTable,
        const FName& ResultPartRowName
    );
};

#include "CraftingSystem.h"
#include "RoguelikeFPS/WeaponPart/PartItem.h"
#include "ResourceItem.h"
#include "RoguelikeFPS/WeaponPart/PartData.h"
#include "Engine/DataTable.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

void UCraftingSystem::DecomposePart(UPartItem* Part, TArray<UResourceItem*>& PlayerResources)
{
    if (!Part) return;

    int32 BaseAmount = 1;
    switch (Part->PartGrade)
    {
    case EPartGrade::Nomal: BaseAmount = 1; break;
    case EPartGrade::Rare: BaseAmount = 2; break;
    case EPartGrade::Hero: BaseAmount = 3; break;
    case EPartGrade::Legend: BaseAmount = 5; break;
    }

    UResourceItem* NewRes = NewObject<UResourceItem>(this, UResourceItem::StaticClass());
    NewRes->ItemName = Part->PartName; 
    NewRes->Amount = BaseAmount;

    PlayerResources.Add(NewRes);

    UE_LOG(LogTemp, Log, TEXT("Decomposed Part: %s -> %d Resource"), *Part->PartName.ToString(), BaseAmount);
}

bool UCraftingSystem::CraftPart(
    TArray<UPartItem*>& PlayerParts,
    TArray<UResourceItem*>& PlayerResources,
    const TMap<TSubclassOf<UResourceItem>, int32>& RequiredResources,
    const TMap<TSubclassOf<UPartItem>, int32>& RequiredParts,
    UDataTable* PartDataTable,
    const FName& ResultPartRowName
)
{
    if (!PartDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("No DataTable provided!"));
        return false;
    }

    for (auto& Pair : RequiredParts)
    {
        int32 Count = 0;
        for (auto* Part : PlayerParts)
        {
            if (Part->IsA(Pair.Key))
                Count++;
        }
        if (Count < Pair.Value)
        {
            UE_LOG(LogTemp, Warning, TEXT("Not enough parts for crafting"));
            return false;
        }
    }

    for (auto& Pair : RequiredResources)
    {
        int32 Count = 0;
        for (auto* Res : PlayerResources)
        {
            if (Res->IsA(Pair.Key))
                Count += Res->Amount;
        }
        if (Count < Pair.Value)
        {
            UE_LOG(LogTemp, Warning, TEXT("Not enough resources for crafting"));
            return false;
        }
    }

    for (auto& Pair : RequiredParts)
    {
        int32 Remaining = Pair.Value;
        for (int32 i = PlayerParts.Num() - 1; i >= 0 && Remaining > 0; i--)
        {
            if (PlayerParts[i]->IsA(Pair.Key))
            {
                PlayerParts.RemoveAt(i);
                Remaining--;
            }
        }
    }

    for (auto& Pair : RequiredResources)
    {
        int32 Remaining = Pair.Value;
        for (auto* Res : PlayerResources)
        {
            if (Res->IsA(Pair.Key))
            {
                int32 Deduct = FMath::Min(Remaining, Res->Amount);
                Res->Amount -= Deduct;
                Remaining -= Deduct;
            }
        }
    }

    static const FString ContextString(TEXT("PART CRAFTING SYSTEM"));
    FPartData* PartData = PartDataTable->FindRow<FPartData>(ResultPartRowName, ContextString);
    if (!PartData)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to find part data for %s"), *ResultPartRowName.ToString());
        return false;
    }

    UPartItem* NewPart = NewObject<UPartItem>(this, UPartItem::StaticClass());
    NewPart->InitializeFromData(*PartData);

    PlayerParts.Add(NewPart);

    UE_LOG(LogTemp, Log, TEXT("Crafted new Part: %s"), *NewPart->PartName.ToString());
    return true;
}

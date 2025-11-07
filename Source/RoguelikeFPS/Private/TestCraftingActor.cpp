#include "TestCraftingActor.h"
#include "CraftingSystem.h"
#include "RoguelikeFPS/WeaponPart/PartItem.h"
#include "ResourceItem.h"
#include "RoguelikeFPS/WeaponPart/PartData.h"

ATestCraftingActor::ATestCraftingActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ATestCraftingActor::BeginPlay()
{
    Super::BeginPlay();

    UCraftingSystem* CraftingSystem = NewObject<UCraftingSystem>();
    TArray<UPartItem*> PlayerParts;
    TArray<UResourceItem*> PlayerResources;

    FName PartRowName = "Muzzle_01";
    FPartData* PartData = PartDataTable->FindRow<FPartData>(PartRowName, TEXT("Test"));
    if (PartData)
    {
        UPartItem* Part = NewObject<UPartItem>();
        Part->InitializeFromData(*PartData);
        PlayerParts.Add(Part);
    }

    if (PlayerParts.Num() > 0)
    {
        CraftingSystem->DecomposePart(PlayerParts[0], PlayerResources);
    }

    if (PlayerResources.Num() > 0 && PlayerParts.Num() > 0)
    {
        TMap<TSubclassOf<UResourceItem>, int32> ReqRes;
        TMap<TSubclassOf<UPartItem>, int32> ReqParts;
        ReqRes.Add(PlayerResources[0]->GetClass(), 1);
        ReqParts.Add(PlayerParts[0]->GetClass(), 1);

        CraftingSystem->CraftPart(PlayerParts, PlayerResources, ReqRes, ReqParts, PartDataTable, PartRowName);
    }
}

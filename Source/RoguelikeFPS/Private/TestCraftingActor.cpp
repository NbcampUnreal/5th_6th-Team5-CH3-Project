//#include "TestCraftingActor.h"
//#include "CraftingSystem.h"
//#include "ItemBase.h"
//#include "ItemData.h"
//#include "Engine/DataTable.h"
//#include "ResourceItem.h"
//
//ATestCraftingActor::ATestCraftingActor()
//{
//    PrimaryActorTick.bCanEverTick = false;
//}
//
//void ATestCraftingActor::BeginPlay()
//{
//    Super::BeginPlay();
//
//    if (!ItemDataTable)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("[CraftTest] No ItemDataTable assigned!"));
//        return;
//    }
//
//    UCraftingSystem* CraftingSystem = NewObject<UCraftingSystem>();
//
//    TArray<UItemBase*> PlayerInventory;
//    TArray<UItemBase*> PlayerResources;
//
//    FName TestRowName = "Muzzle_01";  // DataTableÀÇ ½ÇÁ¦ RowName
//    const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(TestRowName, TEXT("TestCrafting"));
//
//    if (ItemData)
//    {
//        UItemBase* Part = NewObject<UItemBase>();
//        Part->InitItemData(*ItemData);
//        PlayerInventory.Add(Part);
//        UE_LOG(LogTemp, Log, TEXT("[CraftTest] Added part: %s (Grade: %d)"), *Part->ItemName.ToString(), (uint8)Part->PartGrade);
//    }
//
//    if (PlayerInventory.Num() > 0)
//    {
//        CraftingSystem->DecomposePart(PlayerInventory[0], PlayerResources);
//        UE_LOG(LogTemp, Log, TEXT("[CraftTest] Decomposed %s into %d resource(s)"),
//            *PlayerInventory[0]->ItemName.ToString(), PlayerResources.Num());
//    }
//
//    if (PlayerResources.Num() > 0)
//    {
//        TMap<FName, int32> RequiredResources;
//        RequiredResources.Add(PlayerResources[0]->ItemName, 1);
//
//        FName ResultRowName = "Muzzle_02";
//
//        bool bSuccess = CraftingSystem->CraftPart(PlayerInventory, RequiredResources, ItemDataTable, ResultRowName);
//
//        if (bSuccess)
//        {
//            UE_LOG(LogTemp, Log, TEXT("[CraftTest] Crafted new part: %s"), *ResultRowName.ToString());
//        }
//        else
//        {
//            UE_LOG(LogTemp, Warning, TEXT("[CraftTest] Crafting failed."));
//        }
//    }
//}

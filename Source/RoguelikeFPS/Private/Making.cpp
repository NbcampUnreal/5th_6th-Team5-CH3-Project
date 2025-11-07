//#include "Making.h"
//#include "ItemBase.h"
//#include "Inventory.h"
//
//UMaking::UMaking()
//{
//	PrimaryComponentTick.bCanEverTick = false;
//
//}
//
//void UMaking::Init(UInventory* InInventory)
//{
//    PlayerInventory = InInventory;
//}
//
//bool UMaking::CraftItem(const FName& ItemName)
//{
//    if (!PlayerInventory)
//    {
//        return false;
//    }
//
//    FMakingRecipe* FoundRecipe = Recipes.FindByPredicate([&](const FMakingRecipe& R) { return R.ResultItem == ItemName; });
//    if (!FoundRecipe)
//    {
//        return false;
//    }
//
//    for (FName ReqItem : FoundRecipe->MakingItems)
//    {
//        UItemBase* Item = PlayerInventory->SearchItemName(ReqItem);
//        if (!Item || Item->Amount < 1)
//        {
//            return false;
//        }
//
//    for (FName ReqItem : FoundRecipe->MakingItems)
//    {
//        UItemBase* Item = PlayerInventory->SearchItemName(ReqItem);
//        PlayerInventory->RemoveItem(Item, 1);
//    }
//
//    UItemBase* NewItem = NewObject<UItemBase>(PlayerInventory);
//    NewItem->ItemName = ItemName;
//    NewItem->Amount = 1;
//    PlayerInventory->AddItem(NewItem, 2, Item->ItemName);
//
//    return true;
//}
//

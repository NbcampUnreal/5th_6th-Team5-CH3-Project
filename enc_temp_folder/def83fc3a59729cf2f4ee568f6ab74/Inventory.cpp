#include "Inventory.h"
#include "ItemData.h"
#include "InventoryWidget.h"
#include "GameDataInstance.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"


UInventory::UInventory()
{
	PrimaryComponentTick.bCanEverTick = false;

	Gold = 150;

}

UItemBase* UInventory::AddItem(UItemBase* Item, int32 AddAmount, const FName& ItemName)
{
	if (!Item && ItemName == NAME_None)
	{
		return nullptr;
	}

	UItemBase* ItemAdd = nullptr;
	int32 EndAmount = FMath::Max(AddAmount, 1);

	if (Item)
	{
		ItemAdd = Item;
		if (ItemAdd->Amount <= 0)
			ItemAdd->Amount = EndAmount;
	}
	else if (ItemDataTable)
	{
		if (FItemData* Row = ItemDataTable->FindRow<FItemData>(ItemName, TEXT("AddItem")))
		{
			ItemAdd = NewObject<UItemBase>(this);
			ItemAdd->InitItemData(*Row);
			ItemAdd->Amount = EndAmount;
		}
	}

	if (!ItemAdd)
	{
		return nullptr;
	}

	for (UItemBase* InItem : InventoryItems)
	{
		if (InItem && InItem->ItemNumber == ItemAdd->ItemNumber)
		{
			InItem->Amount += ItemAdd->Amount;
			OnInventoryUpdated.Broadcast();
			return InItem;
		}
	}

	InventoryItems.Add(ItemAdd);
	OnInventoryUpdated.Broadcast();

	if (InventoryUI)
	{
		InventoryUI->UpdateUI();
	}

	return ItemAdd;
}
bool UInventory::RemoveItem(UItemBase* Item, int32 Amount)
{
	if (!Item || Amount <= 0)
	{
		return false;
	}
	for (int32 i = 0; i < InventoryItems.Num(); i++)
	{
		UItemBase* RemoveItem = InventoryItems[i];
		if (RemoveItem && RemoveItem->ItemName == Item->ItemName)
		{
			if (RemoveItem->Amount < Amount)
			{
				return false;
			}
			RemoveItem->Amount -= Amount;
			if (RemoveItem->Amount <= 0)
			{
				InventoryItems.RemoveAt(i);
				OnInventoryUpdated.Broadcast();
			}
			return true;
		}
	}
	return false;
}

bool UInventory::BuyItem(UItemBase* Item)
{
	if (!Item || Gold < Item->BuyPrice)
	{
		return false;
	}
	Gold -= Item->BuyPrice;

	for (UItemBase* InvenItem : InventoryItems)
	{
		if (InvenItem && InvenItem->ItemName == Item->ItemName)
		{
			InvenItem->Amount += 1;
			OnGoldChanged.Broadcast(Gold);
			OnInventoryUpdated.Broadcast();
			return true;
		}
	}

	if (ItemDataTable)
	{
		if (FItemData* Row = ItemDataTable->FindRow<FItemData>(Item->ItemName, TEXT("BuyItem")))
		{
			UItemBase* NewItem = NewObject<UItemBase>(this);
			NewItem->InitItemData(*Row);
			NewItem->Amount = 1;
			InventoryItems.Add(NewItem);

			OnGoldChanged.Broadcast(Gold);
			OnInventoryUpdated.Broadcast();

			return true;
		}
	}

	return false;
}

bool UInventory::SellItem(UItemBase* Item, int32 Amount)
{
	if (!Item || Amount <= 0)
	{
		return false;
	}
	UItemBase* InvenItem = SearchItemName(Item->ItemName);
	if (!InvenItem || InvenItem->Amount < Amount)
	{
		return false;
	}
	Gold += InvenItem->SellPrice * Amount;
	RemoveItem(Item, Amount);
	OnGoldChanged.Broadcast(Gold);
	OnInventoryUpdated.Broadcast();

	return true;
}

void UInventory::ShowInventory() const
{
	UE_LOG(LogTemp, Log, TEXT("=== Inventory Detail ==="));
	for (const UItemBase* Item : InventoryItems)
	{
		UE_LOG(LogTemp, Log, TEXT("- %s (Amount : %d)"), *Item->ItemName.ToString(), Item->Amount);
	}
	UE_LOG(LogTemp, Log, TEXT("Current Gold : %d"), Gold);
}
UItemBase* UInventory::SearchItemName(const FName& Name) const
{
	for (UItemBase* Item : InventoryItems)
	{
		if (Item && Item->ItemName == Name)
		{
			return Item;
		}
	}
	return nullptr;
}

UItemBase* UInventory::SearchItemNumber(int32 ItemNumber) const
{
	for (UItemBase* Item : InventoryItems)
	{
		if (Item && Item->ItemNumber == ItemNumber)
		{
			return Item;
		}
	}
	return nullptr;
}

int32 UInventory::GetGold() const
{
	return Gold;
}

void UInventory::SetGold(int32 NewGold)
{
	Gold = FMath::Max(0, NewGold);
	OnGoldChanged.Broadcast(Gold);
}

//void UInventory::SaveInventoryInstance()
//{
//	UGameDataInstance* MyInstance = Cast<UGameDataInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
//
//	if (MyInstance)
//	{
//		MyInstance->InventoryItemName.Empty();
//		for (UItemBase* Item : InventoryItems)
//		{
//			if (Item)
//			{
//				MyInstance->InventoryItemName.Add(Item->ItemName);
//			}
//		}
//		MyInstance->SaveGold = Gold;
//	}
//}
//
//void UInventory::LoadInventoryInstance()
//{
//	UGameDataInstance* MyInstance = Cast<UGameDataInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
//
//	if (MyInstance && ItemDataTable)
//	{
//		InventoryItems.Empty();
//		for (const FName ItemName : MyInstance->InventoryItemName)
//		{
//			FItemData* Row = ItemDataTable->FindRow<FItemData>(ItemName, TEXT(""));
//			if (Row)
//			{
//				UItemBase* NewItem = NewObject<UItemBase>(this, UItemBase::StaticClass());
//				NewItem->InitItemData(*Row);
//				AddItem(NewItem, Row->Amount, Row->ItemName);
//			}
//		}
//		SetGold(MyInstance->SaveGold);
//	}
//}
#include "Inventory.h"

UInventory::UInventory()
{
	PrimaryComponentTick.bCanEverTick = false;

	Gold = 150;

}

void UInventory::AddItem(UItemBase* Item)
{
	if (!Item)
	{
		return;
	}
	for (UItemBase* BuyItem : InventoryItems)
	{
		if (BuyItem && BuyItem->ItemName == Item->ItemName)
		{
			BuyItem->Amount += Item->Amount;
			return;
		}
	}
	UItemBase* NewItem = NewObject<UItemBase>(this);
	NewItem->ItemNumber = Item->ItemNumber;
	NewItem->ItemName = Item->ItemName;
	NewItem->Description = Item->Description;
	NewItem->EffectDescription = Item->EffectDescription;
	NewItem->Amount = Item->Amount;
	NewItem->BuyPrice = Item->BuyPrice;
	NewItem->SellPrice = Item->SellPrice;
	NewItem->ItemType = Item->ItemType;
	NewItem->Thumbnail = Item->Thumbnail;
	InventoryItems.Add(NewItem);

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
			UE_LOG(LogTemp, Log, TEXT("Item Buy : %s, BuyPrice : %d, Current Gold : %d, Amount : %d"),
				*Item->ItemName.ToString(), Item->BuyPrice, Gold, InvenItem->Amount);
			return true;
		}
	}

	UItemBase* NewItem = NewObject<UItemBase>(this);
	NewItem->ItemNumber = Item->ItemNumber;
	NewItem->ItemName = Item->ItemName;
	NewItem->Description = Item->Description;
	NewItem->EffectDescription = Item->EffectDescription;
	NewItem->Amount = 1;
	NewItem->BuyPrice = Item->BuyPrice;
	NewItem->SellPrice = Item->SellPrice;
	NewItem->ItemType = Item->ItemType;
	NewItem->Thumbnail = Item->Thumbnail;
	InventoryItems.Add(NewItem);

	UE_LOG(LogTemp, Log, TEXT("Item Buy : %s, BuyPrice : %d, Current Gold : %d, Amount : %d"), *NewItem->ItemName.ToString(), NewItem->BuyPrice, Gold, NewItem->Amount);
	
	return true;
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

	UE_LOG(LogTemp, Log, TEXT("Sold Item : %s, SellPrice : %d, Current Gold : %d, Amount : %d"),
		*Item->ItemName.ToString(), InvenItem->SellPrice * Amount, Gold, InvenItem ? InvenItem->Amount : 0);
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

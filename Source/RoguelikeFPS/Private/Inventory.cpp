#include "Inventory.h"

UInventory::UInventory()
{
	PrimaryComponentTick.bCanEverTick = false;

	Gold = 150;

}

void UInventory::AddItem(UItemBase* Item)
{
	if (Item)
	{
		Items.Add(Item);
		UE_LOG(LogTemp, Log, TEXT("Item Add : %s"), *Item->ItemName.ToString());
	}
}

bool UInventory::RemoveItem(UItemBase* Item)
{
	if (Item && Items.Contains(Item))
	{
		Items.Remove(Item);
		UE_LOG(LogTemp, Log, TEXT("Item Remove : %s"), *Item->ItemName.ToString());
		return true;
	}
	return false;
}

bool UInventory::BuyItem(UItemBase* Item)
{
	if (!Item || Gold < Item->BuyPrice)
	{
		return false;
	}
	AddItem(Item);
	Gold -= Item->BuyPrice;
	UE_LOG(LogTemp, Log, TEXT("Item Buy : %s, Gold : %d"), *Item->ItemName.ToString(), Gold);
	return true;
}

bool UInventory::SellItem(UItemBase* Item)
{
	if (RemoveItem(Item))
	{
		Gold += Item->SellPrice;
		UE_LOG(LogTemp, Log, TEXT("Item Sell : %s, Gold : %d"), *Item->ItemName.ToString(), Gold);
		return true;
	}
	return false;
}

void UInventory::ShowInventory() const
{
	UE_LOG(LogTemp, Log, TEXT("=== Inventory Detail ==="));
	for (const UItemBase* Item : Items)
	{
		UE_LOG(LogTemp, Log, TEXT("- %s (Amount : %d)"), *Item->ItemName.ToString(), Item->Amount);
	}
	UE_LOG(LogTemp, Log, TEXT("Current Gold : %d"), Gold);
}
UItemBase* UInventory::SearchItemName(const FName& Name) const
{
	for (UItemBase* Item : Items)
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
	for (UItemBase* Item : Items)
	{
		if (Item && Item->ItemNumber == ItemNumber)
		{
			return Item;
		}
	}
	return nullptr;
}
//юс╫ц
void UInventory::TestInventory()
{
	UItemBase* Gun = NewObject<UItemBase>();
	Gun->ItemName = "Gun";
	Gun->Amount = 1;
	Gun->Description = "Basic Gun";
	Gun->BuyPrice = 100;
	Gun->SellPrice = 50;
	Gun->ItemType = "Weapon";

	UItemBase* Potion = NewObject<UItemBase>();
	Potion->ItemName = "Potion";
	Potion->Amount = 5;
	Potion->Description = "Heal Potion";
	Potion->BuyPrice = 20;
	Potion->SellPrice = 10;
	Potion->ItemType = "Potion";

	BuyItem(Gun);
	BuyItem(Potion);

	SellItem(Gun);
	SellItem(Potion);

	ShowInventory();
}

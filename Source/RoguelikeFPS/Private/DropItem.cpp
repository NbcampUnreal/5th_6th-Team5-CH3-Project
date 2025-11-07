// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItem.h"
#include "ItemBase.h"
#include "ItemData.h"
#include "Inventory.h"


// Sets default values
ADropItem::ADropItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Called when the game starts or when spawned
void ADropItem::BeginPlay()
{
	Super::BeginPlay();

	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		inven = MyOwner->GetComponentByClass<UInventory>();
		UE_LOG(LogTemp, Log, TEXT("inventory setting!"));
	}
	UE_LOG(LogTemp, Log, TEXT("inventory setting failed"));
}

// Called every frame
void ADropItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADropItem::DropGoods1(int32 amount)
{
	int32 goods1 = inven->GetGoods1();
	inven->SetGoods1(goods1 + amount);
}

void ADropItem::DropGoods2(int32 amount)
{
	int32 goods2 = inven->GetGoods2();
	inven->SetGoods2(goods2 + amount);
}

void ADropItem::DropTestItem()
{
	UItemBase* TestItem = NewObject<UItemBase>(inven);
	TestItem->ItemName = "ItemName";
	TestItem->Amount = Amount1;
	inven->AddItem(TestItem);
}

void ADropItem::Droptest(int32 amount)
{
	switch (GoodsType)
	{
	case EDropItemType::Goods1:
		DropGoods1(amount);
		break;

	case EDropItemType::Goods2:
		DropGoods2(amount);
		break;

	case EDropItemType::DropItem:
		DropTestItem();
		break;
	}
}





//UItemBase* UInventory::AddItem(UItemBase* Item, int32 AddAmount, const FName& ItemName)
//{
//	UItemBase* ItemAdd = nullptr;
//	int32 EndAmount = FMath::Max(AddAmount, 1);
//	if (Item)
//	{
//		ItemAdd = Item;
//		if (ItemAdd->Amount <= 0)
//		{
//			ItemAdd->Amount = EndAmount;
//		}
//	}
//	else if (ItemName != NAME_None && ItemDataTable)
//	{
//		FItemData* Row = ItemDataTable->FindRow<FItemData>(ItemName, TEXT("AddItem"));
//		if (Row)
//		{
//			ItemAdd = NewObject<UItemBase>(this);
//			ItemAdd->ItemNumber = Row->ItemNumber;
//			ItemAdd->ItemName = Row->ItemName;
//			ItemAdd->Description = Row->Description;
//			ItemAdd->EffectDescription = Row->EffectDescription;
//			ItemAdd->Amount = EndAmount;
//			ItemAdd->BuyPrice = Row->BuyPrice;
//			ItemAdd->SellPrice = Row->SellPrice;
//			ItemAdd->ItemType = Row->ItemType;
//			ItemAdd->Thumbnail = Row->Thumbnail;
//		}
//	}
//	if (!ItemAdd)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Add Item Failed!"));
//		return nullptr;
//	}
//	for (UItemBase* InItem : InventoryItems)
//	{
//		if (InItem && InItem->ItemName == ItemAdd->ItemName)
//		{
//			InItem->Amount += ItemAdd->Amount;
//			return InItem;
//		}
//	}
//	InventoryItems.Add(ItemAdd);
//	if (InventoryUI)
//	{
//		InventoryUI->UpdateUI();
//	}
//	return ItemAdd;
//}

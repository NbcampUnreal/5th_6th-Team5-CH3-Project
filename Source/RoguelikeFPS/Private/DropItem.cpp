// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItem.h"
#include "ItemBase.h"
#include "ItemData.h"
#include "Inventory.h"
#include "Kismet/KismetRenderingLibrary.h"


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

void ADropItem::DropGold()
{
	int32 gold = inven->GetGold();
	inven->SetGold(gold + ADDAmount);
}

void ADropItem::DropStone()
{
	if (!inven) return;


	//FName RowName = TEXT("강화돌");
	//if (FItemData* Row = ItemDataTable->FindRow<FItemData>(RowName, TEXT("강화돌")))
	//{
	//	UItemBase* NewItem = NewObject<UItemBase>();
	//	NewItem->InitItemData(*Row);
	//}

	//UItemBase* TestItem = NewObject<UItemBase>(inven);
	//FItemData testest;

	//testest.ItemName = "강화돌";
	//testest.Amount = 1;
	//testest.ItemType = EItemType::MaterialItem;
	//testest.PartGrade = EPartGrade::Normal;
	//testest.PartType = EPartType::Muzzle;
	//testest.Thumbnail = UKismetRenderingLibrary::ImportFileAsTexture2D(this, TEXT("/All/EngineData/Engine/EditorMaterials/Anchor.png"));
	//testest.BuyPrice = 0;
	//testest.SellPrice = 0;

	//TestItem->InitItemData(testest);

	//inven->AddItem(TestItem, TestItem->Amount, FName("강화돌"));
}

void ADropItem::DropParts()
{
	if (!inven) return;

	UItemBase* TestItem = NewObject<UItemBase>(inven);
	FItemData testest;

	testest.ItemName = "Muzzle";
	testest.Amount = 1;
	testest.ItemType = EItemType::PartItem;
	testest.PartGrade = EPartGrade::Normal;
	testest.PartType = EPartType::Muzzle;
	testest.Thumbnail = UKismetRenderingLibrary::ImportFileAsTexture2D(this, TEXT("/All/EngineData/Engine/EditorMaterials/Anchor.png"));
	testest.BuyPrice = 0;
	testest.SellPrice = 0;
	testest.MinDamage = 100;
	testest.MaxDamage = 200;
	testest.BaseAttackSpeed = 100;

	TestItem->InitItemData(testest);
	inven->AddItem(TestItem, TestItem->Amount, FName("Muzzle"));



}

void ADropItem::Drop()
{
	switch (GoodsType)
	{
	case EDropItemType::Gold:
		DropGold();
		break;

	case EDropItemType::Stone:
		DropStone();
		break;

	case EDropItemType::Parts:
		DropParts();
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

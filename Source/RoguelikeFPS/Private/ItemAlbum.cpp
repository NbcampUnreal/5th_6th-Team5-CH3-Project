#include "ItemAlbum.h"
#include "Engine/DataTable.h"
#include "Engine/Engine.h"

// 이름으로 검색
FItemData UItemAlbum::SearchItemName(const FName& Name) const
{
	if (!ItemDataTable)
		return FItemData();

	TArray<FItemData*> AllItems;
	ItemDataTable->GetAllRows<FItemData>(TEXT(""), AllItems);

	for (FItemData* Item : AllItems)
	{
		if (Item && Item->ItemName == Name)
			return *Item;
	}

	return FItemData();
}

// 번호로 검색
FItemData UItemAlbum::SearchItemNumber(int32 ItemNumber) const
{
	if (!ItemDataTable)
		return FItemData();

	TArray<FItemData*> AllItems;
	ItemDataTable->GetAllRows<FItemData>(TEXT(""), AllItems);

	for (FItemData* Item : AllItems)
	{
		if (Item && Item->ItemNumber == ItemNumber)
			return *Item; 
	}

	return FItemData();
}

// 전체 아이템 조회
void UItemAlbum::GetAllItems(TArray<FItemData>& ShowItem) const
{
	if (!ItemDataTable)
		return;

	TArray<FItemData*> AllItems;
	ItemDataTable->GetAllRows<FItemData>(TEXT(""), AllItems);

	ShowItem.Empty();
	for (FItemData* Item : AllItems)
	{
		if (Item)
			ShowItem.Add(*Item);
	}
}

// 테스트용
void UItemAlbum::TestItemAlbum() const
{
	if (!ItemDataTable)
		return;

	
	FItemData Gun = SearchItemName("Gun");
	if (!Gun.ItemName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("Search by Name: %s, ID: %d"), *Gun.ItemName.ToString(), Gun.ItemNumber);
	}

	
	FItemData Potion = SearchItemNumber(2);
	if (!Potion.ItemName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("Search by ID: %d, Name: %s"), Potion.ItemNumber, *Potion.ItemName.ToString());
	}

	
	TArray<FItemData> AllItems;
	GetAllItems(AllItems);
	for (const FItemData& Item : AllItems)
	{
		UE_LOG(LogTemp, Warning, TEXT("All Item: %s (ID: %d)"), *Item.ItemName.ToString(), Item.ItemNumber);
	}
}

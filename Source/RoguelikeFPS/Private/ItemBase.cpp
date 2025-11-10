#include "ItemBase.h"
#include "ItemData.h"
#include "FPSCharacter.h"

void UItemBase::InitItemData(const FItemData& Data)
{
	ItemNumber = Data.ItemNumber;
	ItemName = Data.ItemName;
	ItemType = Data.ItemType;
	PartType = Data.PartType;
	PartGrade = Data.PartGrade;
	Description = FText::FromString(Data.Description);
	Thumbnail = Data.Thumbnail;
	BuyPrice = Data.BuyPrice;
	SellPrice = Data.SellPrice;
	Amount = Data.Amount;

	if (ItemType == EItemType::PartItem)
	{
		BaseDamage = FMath::RandRange(Data.MinDamage, Data.MaxDamage);
		BaseAttackSpeed = FMath::RandRange(Data.MinAttackSpeed, Data.MaxAttackSpeed);
		Ammo = FMath::RandRange(Data.MinAmmo, Data.MaxAmmo);
	}
	else
	{
		BaseDamage = Data.MinDamage;
		BaseAttackSpeed = Data.MinAttackSpeed;
		Ammo = Data.MinAmmo;
	}
}

void UItemBase::ApplyToWeapon(float& OutDamage, int32& OutAmmo, float& OutAttackSpeed) const
{
	if (ItemType != EItemType::PartItem)
	{
		return;
	}
	OutDamage += (OutDamage * (BaseDamage / 100.f));
	OutAmmo += Ammo;
	OutAttackSpeed *= (1 + BaseAttackSpeed / 100.f);

	UE_LOG(LogTemp, Warning, TEXT("[ItemBase] ApplyToWeapon -> %s | Damage: %.1f | Ammo: %d | AS: %.2f"),
		*ItemName.ToString(), OutDamage, OutAmmo, OutAttackSpeed);
}

//if (FItemData* Row = ItemDataTable->FindRow<FItemData>(RowName, TEXT("°­È­µ¹")))
//{
//	UItemBase* NewItem = NewObject<UItemBase>();
//	NewItem->InitItemData(*Row);
//}
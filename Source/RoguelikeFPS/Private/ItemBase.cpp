#include "ItemBase.h"
#include "ItemData.h"

void UItemBase::InitItemData(const FItemData& Data)
{
	ItemNumber = Data.ItemNumber;
	ItemName = Data.ItemName;
	ItemType = Data.ItemType;
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

float UItemBase::ApplyDamage(float BaseValue) const
{
	if (ItemType != EItemType::PartItem)
	{
		return BaseValue;
	}
	return BaseValue + (BaseValue * (BaseDamage / 100.f));
}

int32 UItemBase::ApplyAmmo(int32 BaseValue) const
{
	if (ItemType != EItemType::PartItem)
	{
		return BaseValue;
	}
	return BaseValue + Ammo;
}

float UItemBase::ApplyAttackSpeed(float BaseValue) const
{
	if (ItemType != EItemType::PartItem)
	{
		return BaseValue;
	}
	return BaseValue * (1 + BaseAttackSpeed / 100.f);
}

void UItemBase::ApplyToWeapon(float& InOutDamage, int32& InOutAmmo, float& InOutAttackSpeed) const
{
	if (ItemType != EItemType::PartItem) return;

	InOutDamage = ApplyDamage(InOutDamage);
	InOutAmmo = ApplyAmmo(InOutAmmo);
	InOutAttackSpeed = ApplyAttackSpeed(InOutAttackSpeed);

	UE_LOG(LogTemp, Warning, TEXT("[ItemBase] ApplyToWeapon -> %s | Damage: %.1f | Ammo: %d | AS: %.2f"),
		*ItemName.ToString(), InOutDamage, InOutAmmo, InOutAttackSpeed);
}

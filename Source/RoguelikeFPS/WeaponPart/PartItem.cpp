//#include "RoguelikeFPS/WeaponPart/PartItem.h"
//
//void UPartItem::InitPartData(const FItemData& Data)
//{
//	InitItemData(Data);
//
//	PartType = Data.PartType;
//	PartGrade = Data.PartGrade;
//
//	// ·£´ý °ª ¼¼ÆÃ
//	Damage = FMath::RandRange(Data.MinDamage, Data.MaxDamage);
//	Ammo = FMath::RandRange(Data.MinAmmo, Data.MaxAmmo);
//	AttackSpeed = FMath::RandRange(Data.MinAttackSpeed, Data.MaxAttackSpeed);
//
//	UE_LOG(LogTemp, Log, TEXT("[PartItem] %s | Grade: %d | Damage: %.1f | Ammo: %d | AS: %.1f"),
//		*ItemName.ToString(), (uint8)PartGrade, Damage, Ammo, AttackSpeed);
//}
//
//void UPartItem::ApplyToWeapon(float& InOutDamage, int32& InOutAmmo, float& InOutAttackSpeed) const
//{
//	InOutDamage = ApplyDamage(InOutDamage);
//	InOutAmmo = ApplyAmmo(InOutAmmo);
//	InOutAttackSpeed = ApplyAttackSpeed(InOutAttackSpeed);
//
//	UE_LOG(LogTemp, Log, TEXT("[PartItem] Applied %s | Final Damage: %.1f | Ammo: %d | AttackSpeed: %.1f"),
//		*ItemName.ToString(), InOutDamage, InOutAmmo, InOutAttackSpeed);
//}
//
//float UPartItem::ApplyDamage(float BaseDamage) const
//{
//	return BaseDamage + (BaseDamage * (Damage / 100.0f));
//}
//
//int32 UPartItem::ApplyAmmo(int32 BaseAmmo) const
//{
//	return BaseAmmo + Ammo;
//}
//
//float UPartItem::ApplyAttackSpeed(float BaseAttackSpeed) const
//{
//	return BaseAttackSpeed * (1 + AttackSpeed / 100.0f);
//}
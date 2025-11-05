#include "RoguelikeFPS/WeaponPart/PartItem.h"

void UPartItem::InitializeFromData(const FPartData& Data)
{
	PartName = Data.PartName;
    PartGrade = Data.PartGrade;
	PartType = Data.PartType;

    Damage = FMath::RandRange(Data.MinDamage, Data.MaxDamage);
    Ammo = FMath::RandRange(Data.MinAmmo, Data.MaxAmmo);
    AttackSpeed = FMath::RandRange(Data.MinAttackSpeed, Data.MaxAttackSpeed);

    ItemName = Data.PartName;
    ItemType = "Part";
    Thumbnail = Data.Thumbnail;
    
    UE_LOG(LogTemp, Log, TEXT("Part : %s, Grad : %d, Damage : %.1f, Ammo : %d, AttackSpeed : %.1f"), *PartName.ToString(), (uint8)PartGrade, Damage, Ammo, AttackSpeed);
}

void UPartItem::ApplyToWeapon(float& InOutDamage, int32& InOutAmmo, float& InOutAttackSpeed) const
{
    InOutDamage = ApplyDamage(InOutDamage);
    InOutAmmo = ApplyAmmo(InOutAmmo);
    InOutAttackSpeed = ApplyAttackSpeed(InOutAttackSpeed);

    UE_LOG(LogTemp, Warning, TEXT("Applied Part: %s, Damage: %.2f, Ammo: %d, AttackSpeed: %.2f"),
        *PartName.ToString(), InOutDamage, InOutAmmo, InOutAttackSpeed);
}

float UPartItem::ApplyDamage(float BaseDamage) const
{
    return BaseDamage + (BaseDamage * (Damage / 100.0f));
}

int32 UPartItem::ApplyAmmo(int32 BaseAmmo) const
{
    return BaseAmmo + Ammo;
}

float UPartItem::ApplyAttackSpeed(float BaseAttackSpeed) const
{
    return BaseAttackSpeed * (1 + AttackSpeed / 100.0f);
}

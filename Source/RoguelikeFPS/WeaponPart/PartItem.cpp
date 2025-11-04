#include "WeaponPart/PartItem.h"

void UPartItem::InitializeFromData(const FPartData& Data)
{
    PartGrade = Data.PartGrade;
    PartType = Data.PartType;
    Damage = Data.Damage;
    MaxAmmo = Data.MaxAmmo;
    AttackSpeed = Data.AttackSpeed;
    Thumbnail = Data.Thumbnail;

    ItemName = Data.PartName;
    ItemType = "Part";
}

float UPartItem::ApplyDamage(float BaseDamage) const
{
    return BaseDamage + (BaseDamage * (Damage / 100.0f));
}

int32 UPartItem::ApplyMaxAmmo(int32 BaseAmmo) const
{
    return BaseAmmo + MaxAmmo;
}

float UPartItem::ApplyAttackSpeed(float BaseAttackSpeed) const
{
    return BaseAttackSpeed * (1 + AttackSpeed / 100.0f);
}

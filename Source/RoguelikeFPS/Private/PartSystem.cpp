#include "PartSystem.h"
#include "Inventory.h"
#include "ItemBase.h"
#include "FPSCharacter.h"
#include "Weapon/GunComponent.h"

UPartSystem::UPartSystem()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UPartSystem::BeginPlay()
{
    Super::BeginPlay();

    OwnerCharacter = Cast<AFPSCharacter>(GetOwner());
    if (OwnerCharacter)
    {
        LinkedInventory = OwnerCharacter->Inventory;
        DefaultDamage = OwnerCharacter->GetAttack();
        DefaultAttackSpeed = static_cast<float>(OwnerCharacter->GetAttackSpeed());

        if(UGunComponent* GunComp = OwnerCharacter->FindComponentByClass<UGunComponent>())
        {
            DefaultAmmo = GunComp->GetMaxBulletCount();
        }
    }
}

void UPartSystem::EquipPart(UItemBase* Item, UInventory* Inventory)
{
    if (!Item || !Inventory || !OwnerCharacter)
    {
        return;
    }

    if (Item->ItemType != EItemType::PartItem)
    {
        return;
    }

    UnequipPart(Item->PartType, Inventory);

    switch (Item->PartType)
    {
    case EPartType::Muzzle:
        EquippedParts.Muzzle = Item;
        break;
    case EPartType::Magazin:
        EquippedParts.Magazin = Item;
        break;
    case EPartType::Stock:
        EquippedParts.Stock = Item;
        break;
    case EPartType::HandGrip: 
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Unknown PartType for %s"), *Item->ItemName.ToString());
        return;
    }

    Inventory->RemoveItem(Item, 1);
    RecalculateStats();

    UE_LOG(LogTemp, Log, TEXT("[PartSystem] Equipped %s | Type: %s | Damage: %.1f | Speed: %.2f | Ammo: %d"),
        *Item->ItemName.ToString(),
        *UEnum::GetValueAsString(Item->PartType),
        FinalDamage, FinalAttackSpeed, FinalAmmo);
}
void UPartSystem::UnequipPart(EPartType PartType, UInventory* Inventory)
{
    if (!Inventory || !OwnerCharacter)
    {
        return;
    }
    UItemBase* UnequippedItem = nullptr;

    switch (PartType)
    {
    case EPartType::Muzzle:
        UnequippedItem = EquippedParts.Muzzle;
        EquippedParts.Muzzle = nullptr;
        break;
    case EPartType::Magazin:
        UnequippedItem = EquippedParts.Magazin;
        EquippedParts.Magazin = nullptr;
        break;
    case EPartType::Stock:
        UnequippedItem = EquippedParts.Stock;
        EquippedParts.Stock = nullptr;
        break;
    default:
        return;
    }

    if (UnequippedItem)
    {
        Inventory->AddItem(UnequippedItem, 1, UnequippedItem->ItemName);
        UE_LOG(LogTemp, Log, TEXT("Unequipped %s "), *UnequippedItem->ItemName.ToString());
    }

    RecalculateStats();
}

void UPartSystem::RecalculateStats()
{
    if (!OwnerCharacter)
    {
        return;
    }

    FinalDamage = DefaultDamage;
    FinalAttackSpeed = DefaultAttackSpeed;
    FinalAmmo = DefaultAmmo;

    ApplyEquippedParts();

    OwnerCharacter->SetAttack(FMath::RoundToInt(FinalDamage));
    OwnerCharacter->SetAttackSpeed(FMath::RoundToInt(FinalAttackSpeed));

    if (UGunComponent* GunComp = OwnerCharacter->FindComponentByClass<UGunComponent>())
    {
        GunComp->SetBulletCount(FinalAmmo);
    }
    UE_LOG(LogTemp, Log, TEXT("Final Stats Updated ¡æ Damage: %.1f | AttackSpeed: %.2f | Ammo: %d"),
        FinalDamage, FinalAttackSpeed, FinalAmmo);
}

void UPartSystem::ApplyEquippedParts()
{
    if (EquippedParts.Muzzle)
    {
        EquippedParts.Muzzle->ApplyToWeapon(FinalDamage, FinalAmmo, FinalAttackSpeed);
    }

    if (EquippedParts.Magazin)
    {
        EquippedParts.Magazin->ApplyToWeapon(FinalDamage, FinalAmmo, FinalAttackSpeed);
    }

    if (EquippedParts.Stock)
    {
        EquippedParts.Stock->ApplyToWeapon(FinalDamage, FinalAmmo, FinalAttackSpeed);
    }

}

void UPartSystem::SetPendingPart(UItemBase* Item)
{
    PendingEquipItem = Item;
}
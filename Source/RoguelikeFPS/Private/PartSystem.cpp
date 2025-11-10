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
        UE_LOG(LogTemp, Log, TEXT("[PartSystem] Attached to Character: %s"), *OwnerCharacter->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[PartSystem] OwnerCharacter not found!"));
    }
}

void UPartSystem::EquipPart(UItemBase* Item, UInventory* Inventory)
{
    if (!Item || !Inventory || !OwnerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("[PartSystem] Invalid parameters for EquipPart."));
        return;
    }

    if (Item->ItemType != EItemType::PartItem)
    {
        UE_LOG(LogTemp, Warning, TEXT("[PartSystem] %s is not a Part Item."), *Item->ItemName.ToString());
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
        UE_LOG(LogTemp, Log, TEXT("[PartSystem] HandGrip part equipped (not yet supported)."));
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("[PartSystem] Unknown PartType for %s"), *Item->ItemName.ToString());
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
        return;

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
        UE_LOG(LogTemp, Warning, TEXT("[PartSystem] Unknown PartType in UnequipPart."));
        return;
    }

    if (UnequippedItem)
    {
        Inventory->AddItem(UnequippedItem, 1, UnequippedItem->ItemName);
        UE_LOG(LogTemp, Log, TEXT("[PartSystem] Unequipped %s (Returned to Inventory)"), *UnequippedItem->ItemName.ToString());
    }

    RecalculateStats();
}
void UPartSystem::SetPendingPart(UItemBase* Item)
{
    PendingEquipItem = Item;

    if (Item)
    {
        UE_LOG(LogTemp, Log, TEXT("[PartSystem] Pending equip item set: %s"), *Item->ItemName.ToString());
    }
}

void UPartSystem::RecalculateStats()
{
    if (!OwnerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("[PartSystem] No OwnerCharacter found."));
        return;
    }

    float BaseDamage = OwnerCharacter->GetAttack();
    float BaseAttackSpeed = static_cast<float>(OwnerCharacter->GetAttackSpeed());
    int32 BaseAmmo = 0;

    UGunComponent* GunComp = OwnerCharacter->FindComponentByClass<UGunComponent>();
    if (GunComp)
    {
        BaseAmmo = GunComp->GetMaxBulletCount();
    }

    FinalDamage = BaseDamage;
    FinalAttackSpeed = BaseAttackSpeed;
    FinalAmmo = BaseAmmo;

    ApplyEquippedParts();

    OwnerCharacter->SetAttack(FMath::RoundToInt(BaseDamage + 10));
    OwnerCharacter->SetAttackSpeed(FMath::RoundToInt(FinalAttackSpeed));

    if (GunComp)
    {
        GunComp->SetBulletCount(FinalAmmo);
        UE_LOG(LogTemp, Log, TEXT("[PartSystem] Updated GunComponent Ammo ¡æ %d"), FinalAmmo);
    }

    UE_LOG(LogTemp, Log, TEXT("[PartSystem] Final Stats Updated ¡æ Damage: %.1f | AttackSpeed: %.2f | Ammo: %d"),
        FinalDamage, FinalAttackSpeed, FinalAmmo);
}

void UPartSystem::ApplyEquippedParts()
{
    if (EquippedParts.Muzzle)
        EquippedParts.Muzzle->ApplyToWeapon(FinalDamage, FinalAmmo, FinalAttackSpeed);

    if (EquippedParts.Magazin)
        EquippedParts.Magazin->ApplyToWeapon(FinalDamage, FinalAmmo, FinalAttackSpeed);

    if (EquippedParts.Stock)
        EquippedParts.Stock->ApplyToWeapon(FinalDamage, FinalAmmo, FinalAttackSpeed);
}

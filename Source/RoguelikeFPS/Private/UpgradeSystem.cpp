#include "UpgradeSystem.h"
#include "Inventory.h"
#include "ItemBase.h"
#include "FPSCharacter.h"
#include "Weapon/GunComponent.h"
#include "Kismet/KismetMathLibrary.h"

UUpgradeSystem::UUpgradeSystem()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UUpgradeSystem::BeginPlay()
{
	Super::BeginPlay();

}

bool UUpgradeSystem::CanUpgrade(UItemBase* TargetItem, UItemBase* UpgradeItem, UInventory* Inventory, int32 CostGold)
{
	if (!TargetItem || !UpgradeItem || !Inventory)
	{
		return false;
	}
	if (Inventory->SearchItemName(UpgradeItem->ItemName) == nullptr)
	{
		return false;
	}
	if (Inventory->GetGold() < CostGold)
	{
		return false;
	}
	return true;
}

bool UUpgradeSystem::UpgradeItem(UItemBase* TargetItem, UItemBase* UpgradeItem, UInventory* Inventory, int32 CostGold)
{
	if (!CanUpgrade(TargetItem, UpgradeItem, Inventory, CostGold))
	{
		return false;
	}
	Inventory->RemoveItem(UpgradeItem, 1);
	Inventory->SetGold(Inventory->GetGold() - CostGold);

	float RateToUse = SuccessRate;
	if (RateToUse <= 0.f)
	{
		switch (TargetItem->PartGrade)
		{
		case EPartGrade::Normal:
			RateToUse = 0.8f;
			break;
		case EPartGrade::Rare:
			RateToUse = 0.65f;
			break;
		case EPartGrade::Hero:
			RateToUse = 0.45f;
			break;
		case EPartGrade::Legendary:
			RateToUse = 0.25;
			break;
		default:
			RateToUse = 0.0f;
			break;
		}
	}

	const float RandomValue = UKismetMathLibrary::RandomFloat();
	UE_LOG(LogTemp, Log, TEXT("Upgrade Attempt: RandomValue=%.2f, SuccessRate=%.2f"), RandomValue, RateToUse);

	if (RandomValue > RateToUse)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UpgradeSystem] Upgrade Failed... %s"), *TargetItem->ItemName.ToString());
		return false;
	}

	EPartGrade OldGrade = TargetItem->PartGrade;
	TargetItem->PartGrade = static_cast<EPartGrade>(FMath::Clamp((uint8)TargetItem->PartGrade + 1, 0, 3));

	const float FixedIncrease = 0.10f; 

	float OldDamage = TargetItem->BaseDamage;
	int32 OldAmmo = TargetItem->Ammo;
	float OldAS = TargetItem->BaseAttackSpeed;

	TargetItem->BaseDamage *= 1.10f;
	TargetItem->Ammo = FMath::CeilToInt(TargetItem->Ammo * 1.10f);
	TargetItem->BaseAttackSpeed *= 1.10f;

	AFPSCharacter* OwnerChar = Cast<AFPSCharacter>(Inventory->GetOwner());
	if (OwnerChar)
	{
		// 🔹 캐릭터 스탯 강화 (공격력, 공격속도)
		int32 OldAttack = OwnerChar->GetAttack();
		int32 OldAttackSpeed = OwnerChar->GetAttackSpeed();

		int32 AddedAttack = FMath::RoundToInt(OldDamage * FixedIncrease);
		OwnerChar->SetAttack(OldAttack + AddedAttack);
		OwnerChar->SetAttackSpeed(FMath::CeilToInt(OldAttackSpeed * 1.10f));

		UE_LOG(LogTemp, Warning,
			TEXT("[UpgradeSystem] Character Stats Increased: Attack %d→%d | AttackSpeed %d→%d"),
			OldAttack, OwnerChar->GetAttack(),
			OldAttackSpeed, OwnerChar->GetAttackSpeed());

		// 🔹 총기 컴포넌트는 탄창 용량만 증가
		if (UGunComponent* GunComp = OwnerChar->FindComponentByClass<UGunComponent>())
		{
			int32 OldMaxBullet = GunComp->GetMaxBulletCount();
			int32 NewMaxBullet = FMath::CeilToInt(OldMaxBullet * 1.10f);
			GunComp->SetBulletCount(NewMaxBullet);

			UE_LOG(LogTemp, Warning,
				TEXT("[UpgradeSystem] Gun Ammo Increased -> MaxAmmo: %d→%d"),
				OldMaxBullet, NewMaxBullet);
		}
	}

	UE_LOG(LogTemp, Warning,
		TEXT("[UpgradeSystem] Upgrade Success -> %s | Grade %d → %d | +10%% | Dmg: %.2f→%.2f | Ammo: %d→%d | AS: %.2f→%.2f"),
		*TargetItem->ItemName.ToString(),
		(uint8)OldGrade, (uint8)TargetItem->PartGrade,
		OldDamage, TargetItem->BaseDamage,
		OldAmmo, TargetItem->Ammo,
		OldAS, TargetItem->BaseAttackSpeed);

	return true;
}
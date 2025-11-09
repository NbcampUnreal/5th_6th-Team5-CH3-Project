#include "UpgradeSystem.h"
#include "Inventory.h"
#include "ItemBase.h"
#include "Weapon/GunComponent.h"
#include "FPSCharacter.h"
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

	//AFPSCharacter* OwnerChar = Cast<AFPSCharacter>(Inventory->GetOwner());
	//if (OwnerChar)
	//{
	//	int32 OldAttack = OwnerChar->GetAttack();
	//	int32 AddedAttack = FMath::RoundToInt(OldDamage * FixedIncrease);
	//	OwnerChar->SetAttack(OldAttack + AddedAttack);

	//	UE_LOG(LogTemp, Warning, TEXT("[UpgradeSystem] Character Attack Increased: %d → %d"), OldAttack, OwnerChar->GetAttack());

	//	if (UGunComponent* GunComp = OwnerChar->FindComponentByClass<UGunComponent>())
	//	{
	//		float OldGunDamage = GunComp->_Status.AttackPoint;
	//		GunComp->_Status.AttackPoint *= 1.10f;

	//		int32 OldMaxBullet = GunComp->_Status.MaxBulletCount;
	//		GunComp->_Status.MaxBulletCount = FMath::CeilToInt(OldMaxBullet * 1.10f);

	//		float OldGunAS = GunComp->_Status.AttackSpeed;
	//		GunComp->_Status.AttackSpeed *= 1.10f;

	//		UE_LOG(LogTemp, Warning,
	//			TEXT("[UpgradeSystem] Gun Enhanced -> Dmg: %.2f→%.2f | MaxAmmo: %d→%d | AS: %.2f→%.2f"),
	//			OldGunDamage, GunComp->_Status.AttackPoint,
	//			OldMaxBullet, GunComp->_Status.MaxBulletCount,
	//			OldGunAS, GunComp->_Status.AttackSpeed);
	//	}
	//}

	//UE_LOG(LogTemp, Warning,
	//	TEXT("[UpgradeSystem] Upgrade Success -> %s | Grade %d → %d | +10%% | Dmg: %.2f→%.2f | Ammo: %d→%d | AS: %.2f→%.2f"),
	//	*TargetItem->ItemName.ToString(),
	//	(uint8)OldGrade, (uint8)TargetItem->PartGrade,
	//	OldDamage, TargetItem->BaseDamage,
	//	OldAmmo, TargetItem->Ammo,
	//	OldAS, TargetItem->BaseAttackSpeed);

	return true;
}
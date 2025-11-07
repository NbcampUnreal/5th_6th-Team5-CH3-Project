#include "UpgradeSystem.h"
#include "Inventory.h"
#include "RoguelikeFPS/WeaponPart/PartItem.h"
#include "Kismet/KismetMathLibrary.h"

UUpgradeSystem::UUpgradeSystem()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UUpgradeSystem::BeginPlay()
{
	Super::BeginPlay();

}

bool UUpgradeSystem::CanUpgrade(UPartItem* TargetItem, UItemBase* UpgradeItem, UInventory* Inventory, int32 CostGold)
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

bool UUpgradeSystem::UpgradeItem(UPartItem* TargetItem, UItemBase* UpgradeItem, UInventory* Inventory, int32 CostGold)
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
		case EPartGrade::Nomal:
			RateToUse = 0.8f;
			break;
		case EPartGrade::Rare:
			RateToUse = 0.65f;
			break;
		case EPartGrade::Hero:
			RateToUse = 0.45f;
			break;
		case EPartGrade::Legend:
			RateToUse = 0.25;
			break;
		default:
			RateToUse = 0.0f;
			break;
		}
	}

	const float RandomValue = UKismetMathLibrary::RandomFloat();
	UE_LOG(LogTemp, Log, TEXT("Upgrade Attempt: RandomValue=%.2f, SuccessRate=%.2f"), RandomValue, RateToUse);

	if (RandomValue <= RateToUse)
	{
		TargetItem->PartGrade = static_cast<EPartGrade>(FMath::Clamp((uint8)TargetItem->PartGrade + 1, 0, 3));
		UE_LOG(LogTemp, Log, TEXT("Upgrade Success! Part : %s, New Grade : %d"), *TargetItem->PartName.ToString(), (uint8)TargetItem->PartGrade);
		return true;
	}

	UE_LOG(LogTemp, Log, TEXT("Upgrade Failed Part : %s"), *TargetItem->PartName.ToString());
	return false;
}

//테스트용
void UUpgradeSystem::TestPartUpgrade(UInventory* Inventory)
{
	if (!Inventory)
	{
		return;
	}
	// 파츠 생성
	UPartItem* Part = NewObject<UPartItem>(Inventory);
	Part->PartName = "TestBarrel";
	Part->PartGrade = EPartGrade::Nomal;

	// 재료 생성
	UItemBase* Stone = NewObject<UItemBase>(Inventory);
	Stone->ItemName = "UpgradeStone";
	Stone->Amount = 5;

	Inventory->AddItem(Part, 1, NAME_None);
	Inventory->AddItem(Stone, 5, NAME_None);
	Inventory->SetGold(500);

	int32 GoldCost = 100;

	UE_LOG(LogTemp, Log, TEXT("=== Start Part Upgrade Test ==="));
	for (int i = 0; i < 5; i++)
	{
		UpgradeItem(Part, Stone, Inventory, GoldCost);
		UE_LOG(LogTemp, Log, TEXT("After attempt %d: Gold=%d, PartGrade=%d"),
			i + 1, Inventory->GetGold(), (uint8)Part->PartGrade);
	}
}
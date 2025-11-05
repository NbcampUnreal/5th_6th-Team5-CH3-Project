#include "RoguelikeFPS/WeaponPart/TestWeapon.h"
#include "RoguelikeFPS/WeaponPart/PartItem.h"

ATestWeapon::ATestWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ATestWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (PartDataTable)	//테스트용
	{
		if (MuzzleRow != NAME_None)
		{
			if (const FPartData* Row = PartDataTable->FindRow<FPartData>(MuzzleRow, TEXT("")))
			{
				MuzzlePart = NewObject<UPartItem>();
				MuzzlePart->InitializeFromData(*Row);
			}
		}

		if (MagazineRow != NAME_None)
		{
			if (const FPartData* Row = PartDataTable->FindRow<FPartData>(MagazineRow, TEXT("")))
			{
				MagazinPart = NewObject<UPartItem>();
				MagazinPart->InitializeFromData(*Row);
			}
		}

		if (StockRow != NAME_None)
		{
			if (const FPartData* Row = PartDataTable->FindRow<FPartData>(StockRow, TEXT("")))
			{
				StockPart = NewObject<UPartItem>();
				StockPart->InitializeFromData(*Row);
			}
		}
	}
	
	FinalDamage = BaseDamage;
	FinalAmmo = BaseAmmo;
	FinalAttackSpeed = BaseAttackSpeed;
	
	if (MuzzlePart)
	{
		MuzzlePart->ApplyToWeapon(FinalDamage, FinalAmmo, FinalAttackSpeed);
	}
	if (MagazinPart)
	{
		MagazinPart->ApplyToWeapon(FinalDamage, FinalAmmo, FinalAttackSpeed);
	}
	if (StockPart)
	{
		StockPart->ApplyToWeapon(FinalDamage, FinalAmmo, FinalAttackSpeed);
	}


	UE_LOG(LogTemp, Warning, TEXT("=== Final Weapon Stats ==="));
	UE_LOG(LogTemp, Warning, TEXT("Damage: %.2f"), FinalDamage);
	UE_LOG(LogTemp, Warning, TEXT("Ammo: %d"), FinalAmmo);
	UE_LOG(LogTemp, Warning, TEXT("AttackSpeed: %.2f"), FinalAttackSpeed);
}


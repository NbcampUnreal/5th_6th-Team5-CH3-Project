//#include "RoguelikeFPS/WeaponPart/TestWeapon.h"
//#include "ItemData.h"
//
//ATestWeapon::ATestWeapon()
//{
//	PrimaryActorTick.bCanEverTick = false;
//
//}
//
//void ATestWeapon::BeginPlay()
//{
//	Super::BeginPlay();
//
//	if (PartDataTable)	//테스트용
//	{
//		if (MuzzleRow != NAME_None)
//		{
//			if (const FItemData* Row = PartDataTable->FindRow<FItemData>(MuzzleRow, TEXT("")))
//			{
//				MuzzlePart = NewObject<UItemBase>();
//				MuzzlePart->InitItemData(*Row);
//			}
//		}
//
//		if (MagazineRow != NAME_None)
//		{
//			if (const FItemData* Row = PartDataTable->FindRow<FItemData>(MagazineRow, TEXT("")))
//			{
//				MagazinPart = NewObject<UItemBase>();
//				MagazinPart->InitItemData(*Row);
//			}
//		}
//
//		if (StockRow != NAME_None)
//		{
//			if (const FItemData* Row = PartDataTable->FindRow<FItemData>(StockRow, TEXT("")))
//			{
//				StockPart = NewObject<UItemBase>();
//				StockPart->InitItemData(*Row);
//			}
//		}
//	}
//	
//	FinalDamage = BaseDamage;
//	FinalAmmo = BaseAmmo;
//	FinalAttackSpeed = BaseAttackSpeed;
//	
//	if (MuzzlePart)
//	{
//		MuzzlePart->ApplyToWeapon(FinalDamage, FinalAmmo, FinalAttackSpeed);
//	}
//	if (MagazinPart)
//	{
//		MagazinPart->ApplyToWeapon(FinalDamage, FinalAmmo, FinalAttackSpeed);
//	}
//	if (StockPart)
//	{
//		StockPart->ApplyToWeapon(FinalDamage, FinalAmmo, FinalAttackSpeed);
//	}
//
//
//	UE_LOG(LogTemp, Warning, TEXT("=== Final Weapon Stats ==="));
//	UE_LOG(LogTemp, Warning, TEXT("Damage: %.2f"), FinalDamage);
//	UE_LOG(LogTemp, Warning, TEXT("Ammo: %d"), FinalAmmo);
//	UE_LOG(LogTemp, Warning, TEXT("AttackSpeed: %.2f"), FinalAttackSpeed);
//}
//

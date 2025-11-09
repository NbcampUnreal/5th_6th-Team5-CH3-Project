//#pragma once
//
//#include "CoreMinimal.h"
//#include "ItemBase.h"
//#include "PartItem.generated.h"
//
//
//UCLASS(BlueprintType)
//class ROGUELIKEFPS_API UPartItem : public UItemBase
//{
//	GENERATED_BODY()
//	
//public:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
//	EPartType PartType;  // 파츠 종류
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
//	float Damage;   // 공격력
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
//	int32 Ammo;     // 추가 탄창
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
//	float AttackSpeed; // 공속
//
//	void InitPartData(const FItemData& Data);
//
//	void ApplyToWeapon(float& InOutDamage, int32& InOutAmmo, float& InOutAttackSpeed) const;
//
//private:
//	float ApplyDamage(float BaseDamage) const;
//	int32 ApplyAmmo(int32 BaseAmmo) const;
//	float ApplyAttackSpeed(float BaseAttackSpeed) const;
//};

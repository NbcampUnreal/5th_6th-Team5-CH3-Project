#pragma once

#include "CoreMinimal.h"
#include "PartData.h"
#include "ItemBase.h"
#include "PartItem.generated.h"


UCLASS(BlueprintType)
class ROGUELIKEFPS_API UPartItem : public UItemBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	FName PartName;//이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	EPartGrade PartGrade;//등급
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	EPartType PartType;//종류

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	float Damage;//데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	int32 Ammo;//장탄
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	float AttackSpeed;//공격속도

	float ApplyDamage(float BaseDamage) const;//데미지
	int32 ApplyAmmo(int32 BaseAmmo) const;//탄창
	float ApplyAttackSpeed(float BaseAttackSpeed) const;//공격속도

	void InitializeFromData(const FPartData& Data);
	void ApplyToWeapon(float& InOutDamage, int32& InOutAmmo, float& InOutAttackSpeed) const;
};

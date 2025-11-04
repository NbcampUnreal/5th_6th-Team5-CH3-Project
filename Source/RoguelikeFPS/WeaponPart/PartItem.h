#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "PartData.h"
#include "PartItem.generated.h"


UCLASS()
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
	float Damage;	//데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	int32 MaxAmmo;	//장탄수 증가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	float AttackSpeed; //공격속도 증가

	void InitializeFromData(const FPartData& Data);

	float ApplyDamage(float BaseDamage) const;
	int32 ApplyMaxAmmo(int32 BaseAmmo) const;
	float ApplyAttackSpeed(float BaseAttackSpeed) const;
};

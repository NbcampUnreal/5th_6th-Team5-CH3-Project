#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PartData.generated.h"

UENUM(BlueprintType)
enum class EPartGrade : uint8
{
	Nomal UMETA(DisplayName = "Nomal"),
	Rare UMETA(DisplayName = "Rare"),
	Hero UMETA(DisplayName = "Hero"),
	Legend UMETA(DisplayName = "Legend")
};
UENUM(BlueprintType)
enum class EPartType : uint8
{
	Muzzle UMETA(DisplayName = "Muzzle"),
	Magazin UMETA(DisplayName = "Magazin"),
	Stock UMETA(DisplayName = "Stock"),
	HandGrip UMETA(DisplayName = "HandGrip")
};
USTRUCT(BlueprintType)
struct FPartData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PartName;//이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPartGrade PartGrade;//등급
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPartType PartType;//종류

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	float MinDamage;	//최소 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	float MaxDamage;	//최대 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	int32 MinAmmo;	//최소 장탄수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	int32 MaxAmmo;	//최대 장탄수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	float MinAttackSpeed; //최소 공격속도 증가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	float MaxAttackSpeed; //최대 공격속도 증가

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;
};

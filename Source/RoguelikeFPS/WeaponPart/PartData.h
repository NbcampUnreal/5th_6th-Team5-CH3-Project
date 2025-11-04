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
	Muzzle UMETA(DisplayName = "총구"),
	Magazin UMETA(DisplayName = "탄창"),
	Stock UMETA(DisplayName = "개머리판"),
	HandGrip UMETA(DisplayName = "손잡이")
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;	//데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxAmmo;	//장탄수 증가
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeed; //공격속도 증가
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//float ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;
};

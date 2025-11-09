#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

UENUM()
enum class EItemType : uint8
{
	UseItem UMETA(DisplayName = "UseItem"),
	MaterialItem UMETA(DisplayName = "MaterialItem"),
	PartItem UMETA(DisplayName = "PartItem")
};
UENUM()
enum class EPartGrade : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Rare UMETA(DisplayName = "Rare"),
	Hero UMETA(DisplayName = "Hero"),
	Legendary UMETA(DisplayName = "Legendary")
};
UENUM()
enum class EPartType : uint8
{
	None UMETA(Display = "None"),
	Muzzle UMETA(DisplayName = "Muzzle"),
	Magazin UMETA(DisplayName = "Magazin"),
	Stock UMETA(DisplayName = "Stock"),
	HandGrip UMETA(DisplayName = "HandGrip")
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:

	//ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	int32 ItemNumber;
	//이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	FName ItemName;
	//설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	FString Description;
	//아이템 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	EItemType ItemType = EItemType::MaterialItem;
	//수량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	int32 Amount;
	//구입 가격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	int32 BuyPrice;
	//판매 가격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	int32 SellPrice;
	//이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	UTexture2D* Thumbnail;
	//파츠 등급
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	EPartGrade PartGrade = EPartGrade::Normal;
	//파츠 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	EPartType PartType = EPartType::Muzzle;
	//기본 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	float BaseDamage = 0.0f;
	//기본 공격속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	float BaseAttackSpeed = 0.0f;
	//기본 장탄
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	int32 Ammo = 0;
	//범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	float MinDamage = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	float MaxDamage = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	float MinAttackSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	float MaxAttackSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	float MinAmmo = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part")
	float MaxAmmo = 0.0f;

	//제작
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	TMap<FName, int32> CraftingItems;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	TMap<FName, int32> DestroyItems;
};

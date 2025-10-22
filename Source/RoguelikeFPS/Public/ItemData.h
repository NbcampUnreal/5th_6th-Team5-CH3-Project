#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

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
	//효과 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	FString EffectDescription;
	//수량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	int32 Amount;
	//구입 가격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	int32 BuyPrice;
	//판매 가격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemS")
	int32 SellPrice;
	//아이템 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemS")
	FString ItemType;

};

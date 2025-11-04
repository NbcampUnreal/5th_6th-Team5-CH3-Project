#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemBase.generated.h"

UCLASS()
class ROGUELIKEFPS_API UItemBase : public UObject
{
	GENERATED_BODY()
	
public:

	UItemBase();

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	int32 SellPrice;
	//아이템 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	FString ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	UTexture2D* Thumbnail;
};

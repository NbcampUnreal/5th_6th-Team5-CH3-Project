#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.h"
#include "UObject/NoExportTypes.h"
#include "ItemAlbum.generated.h"

UCLASS(Blueprintable)
class ROGUELIKEFPS_API UItemAlbum : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemAlbum")
	UDataTable* ItemDataTable;

	// 이름/ID로 검색 (값 반환)
	UFUNCTION(BlueprintCallable, Category = "ItemAlbum")
	FItemData SearchItemName(const FName& Name) const;

	UFUNCTION(BlueprintCallable, Category = "ItemAlbum")
	FItemData SearchItemNumber(int32 ItemNumber) const;

	// 전체 아이템 조회
	UFUNCTION(BlueprintCallable, Category = "ItemAlbum")
	void GetAllItems(TArray<FItemData>& ShowItem) const;

	// 테스트용
	UFUNCTION(BlueprintCallable, Category = "ItemAlbum")
	void TestItemAlbum() const;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ItemBase.h"
#include "TestWeapon.generated.h"

class UPartItem;

UCLASS()
class ROGUELIKEFPS_API ATestWeapon : public AActor
{
	GENERATED_BODY()

public:
	ATestWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float BaseDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	int32 BaseAmmo = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float BaseAttackSpeed = 1.0f;

	//파츠
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Parts")
	UItemBase* MuzzlePart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Parts")
	UItemBase* MagazinPart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Parts")
	UItemBase* StockPart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Parts")
	FName MuzzleRow;//테스트용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Parts")
	FName MagazineRow;//테스트용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Parts")
	FName StockRow;//테스트용

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	float FinalDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	int32 FinalAmmo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	float FinalAttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PartData")
	UDataTable* PartDataTable;

protected:
	virtual void BeginPlay() override;

};

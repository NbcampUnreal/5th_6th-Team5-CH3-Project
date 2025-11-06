// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponComponent.h"
#include "GunStatus.h"
#include "Projectile.h"
#include "GunComponent.generated.h"

/**
 * 
 */
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProjectileSpawn, AProjectile*, Projectile);
DECLARE_MULTICAST_DELEGATE_OneParam(FProjectileSpawn, AProjectile*);

class UProjectileComponent;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROGUELIKEFPS_API UGunComponent : public UWeaponComponent
{
	GENERATED_BODY()
	

public:


	UGunComponent();
	~UGunComponent();

	//UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FProjectileSpawn ProjectileSpawn;

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> _ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, meta = (AllowPrivateAccess = "true"))
	FGunStatus _Status;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	int32 CurrentBulletCount = 0;

	UPROPERTY()
	FTimerHandle _GunTimerHandle;

	UPROPERTY()
	bool CanAttack; 

public:
	virtual void DoAttack() override;

	void Fire();

	float ReturnDamage();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	FRotator CalculateSapwnRotaion();

	void InitSpawnProjectile(AProjectile* proejectile);
	void ReloadBullet();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponComponent.h"
#include "GunStatus.h"
#include "Projectile.h"
#include "Weapon/GunDataAsset.h"
#include "GunComponent.generated.h"

/**
 * 
 */
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProjectileSpawn, AProjectile*, Projectile);
DECLARE_MULTICAST_DELEGATE_OneParam(FProjectileSpawn, AProjectile*);

class UProjectileComponent;
class USpringArmComponent;
class UCameraComponent;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, meta = (AllowPrivateAccess = "true"))
	int32 CurrentBulletCount = 0;

	UPROPERTY()
	FTimerHandle _GunTimerHandle;

	UPROPERTY()
	FTimerHandle _ReloadTimerHandle;

	UPROPERTY()
	bool CanAttack; 

	UPROPERTY()
	bool _IsReloading = false;

public:
	virtual void DoAttack() override;

	void Fire();

	float ReturnDamage();

	void AddBullet(int32 amount);
	int32 GetBulletCount() { return CurrentBulletCount; };
	void SetBulletCount(int32 count);

	int32 GetMaxBulletCount() { return _Status.MaxBulletCount; };

	float GetRealodTime() { return _Status.ReloadTime; };
	void SetRealodTime(float time);

	void SetProjectilesPerShot(int32 count);

	float GetAttackPoint() { return _Status.AttackPoint; };
	void SetAttackPoint(float attackpoint);

	void IncreaseHeadShotMultiplier(float value);
	
	void SetData(UGunDataAsset* data);
	void Assign(UGunDataAsset* data);
	
	virtual void AttachWeapon(ACharacter* TargetCharacter) override;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnRegister() override;

	
private:
	FRotator CalculateSapwnRotaion();

	void InitSpawnProjectile(AProjectile* proejectile);
	void ReloadBullet();
	void SpawnProejectile();

	void ZoomIn();
	void ZoomOut();
};

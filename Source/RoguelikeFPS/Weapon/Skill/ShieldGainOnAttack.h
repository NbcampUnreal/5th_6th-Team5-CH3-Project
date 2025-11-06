// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponSkillComponent.h"
#include "ShieldGainOnAttack.generated.h"

/**
 * 
 */
class AProjectile;
UCLASS()
class ROGUELIKEFPS_API UShieldGainOnAttack : public UWeaponSkillComponent
{
	GENERATED_BODY()
	
public:
	
protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flamethrower, meta = (ClampMin = "1.0"), meta = (AllowPrivateAccess = "true"))
	int32 _ShieldAmountGained = 10;

public:
	virtual void SetUp() override;

protected:

private:
	UFUNCTION()
	void Projectile_AddDynamic(AProjectile* projectile);

	void GetShield(ACharacter* Victim);
};

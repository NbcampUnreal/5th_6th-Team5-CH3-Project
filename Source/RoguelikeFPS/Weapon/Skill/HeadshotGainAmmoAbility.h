// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponSkillComponent.h"
#include "HeadshotGainAmmoAbility.generated.h"

/**
 * 
 */
class AProjectile;

UCLASS()
class ROGUELIKEFPS_API UHeadshotGainAmmoAbility : public UWeaponSkillComponent
{
	GENERATED_BODY()

public:

protected:

private:

public:

protected:

private:
	virtual void SetUp() override;
	void Projectile_AddDynamic(AProjectile* projectile);
	void GainBullet();
};

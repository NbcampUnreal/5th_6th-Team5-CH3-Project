// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponSkillComponent.h"
#include "UnlimitedAmmoAbility.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKEFPS_API UUnlimitedAmmoAbility : public UWeaponSkillComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	float _BuffDuration = 5.0f;
protected:

private:

public:

protected:

private:
	virtual void GetAmmo(AProjectile* projectile);

	virtual void SetUp() override;

	virtual void Active() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponSkillComponent.h"
#include "ChanceToStunAbility.generated.h"

/**
 * 
 */

class AProjectile;

UCLASS()
class ROGUELIKEFPS_API UChanceToStunAbility : public UWeaponSkillComponent
{
	GENERATED_BODY()

public:

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	float _Chance = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	float _Duration = 1.f;

public:

protected:

private:
	virtual void SetUp();

	void SetProjectile(AProjectile* projectile);
	void ApplyStun(ACharacter* Victim);
};

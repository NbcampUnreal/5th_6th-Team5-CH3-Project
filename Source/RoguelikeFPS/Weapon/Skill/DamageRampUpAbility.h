// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponSkillComponent.h"
#include "DamageRampUpAbility.generated.h"

/**
 * 
 */

class AProjectile;

UCLASS()
class ROGUELIKEFPS_API UDamageRampUpAbility : public UWeaponSkillComponent
{
	GENERATED_BODY()
	
public:


protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	int32 _MaxStack = 30;

	UPROPERTY()
	int32 _CurrentStack = 0;

	UPROPERTY()
	FTimerHandle _BuffDurationTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	float _BonusDamagePerStack = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	float _BuffDuration = 1.0f;


public:

protected:

private:
	virtual void SetUp() override;

	virtual void Active() override;

	void SetProjectileDamge(AProjectile* projectile);

	void GetStack();
	void ResetStack();
};

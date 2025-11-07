// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponSkillComponent.h"
#include "UnlimitedAmmoAbility.generated.h"

/**
 * 
 */

class AProjectile;

UCLASS()
class ROGUELIKEFPS_API UUnlimitedAmmoAbility : public UWeaponSkillComponent
{
	GENERATED_BODY()

public:
	
protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	float _BuffDuration = 5.0f;

	UPROPERTY()
	FTimerHandle _BuffDurationTimerHandle;

	UPROPERTY()
	bool IsBuffActive = false;

public:

protected:

private:
	virtual void GetAmmo();

	virtual void SetUp() override;

	virtual void Active() override;

	void EndBuff();
};

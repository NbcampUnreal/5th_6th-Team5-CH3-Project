// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponSkillComponent.h"
#include "DoubledAmmoBuff.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKEFPS_API UDoubledAmmoBuff : public UWeaponSkillComponent
{
	GENERATED_BODY()

public:

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	float _BuffDuration = 5.0f;

	UPROPERTY()
	FTimerHandle _BuffTimerHandle;

public:

protected:

private:
	virtual void Active() override;
	bool ApplyDoubleAmmoEffect(bool bEnableDouble);
	void EndBuff();
};

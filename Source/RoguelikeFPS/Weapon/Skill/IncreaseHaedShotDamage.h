// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponSkillComponent.h"
#include "IncreaseHaedShotDamage.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKEFPS_API UIncreaseHaedShotDamage : public UWeaponSkillComponent
{
	GENERATED_BODY()
	

public:

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	float _HeadShotMultiplier = 1;
public:

protected:

private:
	virtual void SetUp() override;
};

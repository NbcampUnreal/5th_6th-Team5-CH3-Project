// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponSkillComponent.h"
#include "InvincibilityAbility.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKEFPS_API UInvincibilityAbility : public UWeaponSkillComponent
{
	GENERATED_BODY()

public:

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	float _BuffDuration = 5.0f;

public:

protected:

private:
	virtual void Active() override;
	
};

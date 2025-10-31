// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponSkillComponent.h"
#include "Flamethrower.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKEFPS_API UFlamethrower : public UWeaponSkillComponent
{
	GENERATED_BODY() 

public:
	

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	float Angle = 60.f;

public:

protected:

private:
	virtual void Active() override;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponSkillComponent.h"
#include "SixShot.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKEFPS_API USixShot : public UWeaponSkillComponent
{
	GENERATED_BODY()
	

public:
	
protected:
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	float _AttackDelay = 0.f;

	UPROPERTY()
	int32 BurstCount = 0;

	UPROPERTY()
	FTimerHandle BurstTimerHandle;
public:

protected:

private:
	virtual void Active() override;
	void BurstTick();
};

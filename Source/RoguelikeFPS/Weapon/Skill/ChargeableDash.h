// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponSkillComponent.h"
#include "ChargeableDash.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKEFPS_API UChargeableDash : public UWeaponSkillComponent
{
	GENERATED_BODY()
	
public:
	UChargeableDash();
protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	int32 _ChargedDashCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	int32 _MaxDashCount = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	float _DashMultifly = 1.0f;

	bool _IsDash = false;
public:

protected:

private:
	virtual void Active() override;

	virtual void SetUp() override;

	void IncreaseDashCharge();

	void StartDash();
	void StopDash();
};

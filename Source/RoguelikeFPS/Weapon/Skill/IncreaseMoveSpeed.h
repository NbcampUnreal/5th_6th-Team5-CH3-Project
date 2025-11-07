// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponSkillComponent.h"
#include "IncreaseMoveSpeed.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKEFPS_API UIncreaseMoveSpeed : public UWeaponSkillComponent
{
	GENERATED_BODY()

public:
	


protected:

private:
	UPROPERTY()
	int32 _OwnerMovingSpeed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	float _BuffDuration = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	float _DashMultifly = 1.2f;

	UPROPERTY()
	FTimerHandle _DashDurationTimerHandle;

public:
	virtual void Active() override;
protected:

private:
	virtual void SetUp() override;

	void SetOwnerSpeed(float speedamount);

	void EndBuff();
};

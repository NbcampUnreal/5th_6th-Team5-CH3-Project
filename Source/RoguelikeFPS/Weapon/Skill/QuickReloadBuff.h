// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponSkillComponent.h"
#include "QuickReloadBuff.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKEFPS_API UQuickReloadBuff : public UWeaponSkillComponent
{
	GENERATED_BODY()
	

public:

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	float _BuffDuration = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	float _ReloadTimeReductionRatio = 0;

	UPROPERTY()
	FTimerHandle _BuffDurationTimerHandle;
public:

protected:

private:
	virtual void Active() override;

	void EndBuff();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponSkillComponent.h"
#include "Weapon/Projectile.h"
#include "Weapon/Skill/ExplosiveActor.h"
#include "ExplosiveBullet.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKEFPS_API UExplosiveBullet : public UWeaponSkillComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	TSubclassOf<AExplosiveActor> _Explosion;

protected:

private:
	UPROPERTY()
	ACharacter* _Instigator;

public:
	virtual void SetUp() override;

protected:

private:
	UFUNCTION()
	void Projectile_AddDynamic(AProjectile* projectile);

	UFUNCTION()
	void Spawn_Explosion(AActor* DestroyedActor);
};

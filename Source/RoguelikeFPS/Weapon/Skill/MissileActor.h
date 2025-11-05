// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile.h"
#include "MissileActor.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKEFPS_API AMissileActor : public AProjectile
{
	GENERATED_BODY()


public:

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	float _AscendSpeed = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	float _HomingAccel = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	float _CruiseSpeed = 5;

	UPROPERTY();
	AActor* _Target;

public:
	void SetTarget(AActor* target);

protected:

private:
	virtual void BeginPlay() override;

	void EnableHoming();
};

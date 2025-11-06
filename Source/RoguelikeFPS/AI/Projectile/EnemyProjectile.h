// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile.h"
#include "EnemyProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKEFPS_API AEnemyProjectile : public AProjectile
{
	GENERATED_BODY()
	
public:
	AEnemyProjectile();

	//void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};

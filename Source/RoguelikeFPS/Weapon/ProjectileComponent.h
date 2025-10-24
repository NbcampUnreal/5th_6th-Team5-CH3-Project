// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "ProjectileComponent.generated.h"

/**
 * 
 */

class UProjectileMovementComponent;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROGUELIKEFPS_API UProjectileComponent : public USphereComponent
{
	GENERATED_BODY()
	

public:
	UProjectileComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Data, meta = (AllowPrivateAccess = "true"))
	float Damage;
protected:
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* _ProjectileMovement;
	
public:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void BeginPlay() override;

	void SetMovementSpeed(float speed);

	//UProjectileMovementComponent* GetProjectileMovement() const { return _ProjectileMovement; }
protected:

private:
};

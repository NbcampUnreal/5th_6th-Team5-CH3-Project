// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponSkillComponent.h"
#include "Components/BoxComponent.h"
#include "Missile.generated.h"

/**
 * 
 */
class AMissileActor;

UCLASS()
class ROGUELIKEFPS_API UMissile : public UWeaponSkillComponent
{
	GENERATED_BODY()

public:
	UMissile();

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flamethrower, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* _Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flamethrower, meta = (AllowPrivateAccess = "true"))
	int32 _MissilesToFireCount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flamethrower, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMissileActor> _TSubMissileActor;

	UPROPERTY()
	ACharacter* _Instigator;

	UPROPERTY()
	TArray<TWeakObjectPtr<ACharacter>> OverlappedCharacters;


public:

protected:

private:
	virtual void Active() override;

	virtual void SetUp() override;

	UFUNCTION()
	void OnCollsionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void CreateCollision();
	void DestroyCollision();
	void SpawnMissiles();
};

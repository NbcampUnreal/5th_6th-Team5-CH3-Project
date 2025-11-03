// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponSkillComponent.h"
#include "ProceduralMeshComponent.h"
#include "NiagaraSystem.h"
#include "Flamethrower.generated.h"

/**
 * 
 */
class ACharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROGUELIKEFPS_API UFlamethrower : public UWeaponSkillComponent
{
	GENERATED_BODY() 

public:
	UFlamethrower();

protected:


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flamethrower, meta = (AllowPrivateAccess = "true"))
	UProceduralMeshComponent* _Collision;

	UPROPERTY()
	float _Width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flamethrower, meta = (ClampMin = "1.0"), meta = (AllowPrivateAccess = "true"))
	float _Height = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flamethrower, meta = (AllowPrivateAccess = "true"))
	float _Length = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flamethrower, meta = (AllowPrivateAccess = "true"))
	float _Angle = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flamethrower, meta = (AllowPrivateAccess = "true"))
	int32 _TickPerSecond = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flamethrower, meta = (AllowPrivateAccess = "true"))
	float _FlamethrowerDuration = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "FX", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* _VFXFlamethrower;

	UPROPERTY()
	UNiagaraComponent* _VFXComponent;

	UPROPERTY()
	ACharacter* _Instigator;

	UPROPERTY()
	TSet<TWeakObjectPtr<ACharacter>> OverlappedCharacters;

	UPROPERTY()
	FTimerHandle DamageOnTickHandle;

public:

protected:

private:
	virtual void Active() override;

	virtual void SetUp() override;

	UFUNCTION()
	void OnCollsionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCollsionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CreateCollisonShape();

	void ApplyDamageOnTick();
};

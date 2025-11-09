// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class UNiagaraComponent;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDestroyed);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamagedEnemy, ACharacter*);
DECLARE_MULTICAST_DELEGATE(FOnHitHead);

UCLASS()
class ROGUELIKEFPS_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();

	FOnDamagedEnemy OnDamagedEnemy;

	FOnHitHead OnHitHead;

	float GetDamage() const { return _Damage; };

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* _ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	USphereComponent* _Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	float _Damage = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	float _HeadShotMultiplier = 1;

	UPROPERTY(EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TSet<FName> HeadBoneNames = { TEXT("head"), TEXT("Head"), TEXT("Head_top") /* 등 프로젝트 본명에 맞춰 */ };

	//UPROPERTY(EditAnywhere, Category = Projectile|VFX, meta = (AllowPrivateAccess = "true"))
	//UNiagaraSystem* RibbonParticle;

	UPROPERTY(EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* RibbonParticleComponent;

private:

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void SetMovementSpeed(float speed);

	void SetDamage(float damage);
	void AddDamage(float damage);

	void SetHeadShotMultiplier(float value);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


private:
	UFUNCTION(BlueprintCallable)
	bool IsHeadBone(FName Bone) const { return HeadBoneNames.Contains(Bone); }
};

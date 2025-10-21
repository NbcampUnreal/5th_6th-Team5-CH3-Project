// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon/PickUpComponent.h"
#include "WeaponComponent.generated.h"

/**
 * 
 */
class ACharacter;

UCLASS()
class ROGUELIKEFPS_API UWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	USoundBase* _AttackSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* _AttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* _AttackMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* _AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	UPickUpComponent* _PickUpComponent;


public:
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void StartAttack();

	/*UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool TryAttachWeapon(ACharacter* TargetCharacter);*/

	UFUNCTION()
	void AttachWeapon(ACharacter* TargetCharacter); // 델리게이트용 (void)

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	//Owner
	ACharacter* _Character;
};

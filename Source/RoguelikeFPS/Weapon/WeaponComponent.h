// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon/PickUpComponent.h"
#include "WeaponSkillComponent.h"
#include "WeaponComponent.generated.h"

/**
 * 
 */
class ACharacter;
class UEnhancedInputComponent;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROGUELIKEFPS_API UWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();

protected:
	//Owner
	ACharacter* _Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	USoundBase* _AttackSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* _AttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* _AttackMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* _AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UWeaponSkillComponent> _TSubSkillComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	FName WeaponSocketName;

	UPROPERTY()
	UWeaponSkillComponent* _SkillComponent;

private:
	
public:
	ACharacter* GetOwnerCharacter() { return _Character;  };

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void DoAttack();

	UFUNCTION()
	void AttachWeapon(ACharacter* TargetCharacter); // 델리게이트용 (void)

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void ActiveSkill();

	UEnhancedInputComponent* GetCharacterEnhancedInputComponent();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


private:

};

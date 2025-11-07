// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WeaponSkillComponent.generated.h"


//UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROGUELIKEFPS_API UWeaponSkillComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponSkillComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	float _CoolDown = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
	float _AttackDamageMultiplier = 1.f;

	UPROPERTY()
	float _Damage = 0.f;

	UPROPERTY()
	bool _isSkillCoolDown = false;

	UPROPERTY()
	FTimerHandle _SkillTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* _SkillAction;

private:

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void Active();
	virtual void SetUp();

protected:
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
	virtual void OnAttachmentChanged() override;
	void SetActionMapping();
	void SetCoolDownTimer();
	void SetDamge();
private:
		
};

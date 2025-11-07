// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Skill/IncreaseMoveSpeed.h"
#include "Weapon/WeaponComponent.h"
#include "FPSCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UIncreaseMoveSpeed::Active()
{
	Super::Active();

	if (_isSkillCoolDown) return;
	_isSkillCoolDown = true;

	SetOwnerSpeed(_OwnerMovingSpeed * _DashMultifly);

	UWorld* const World = GetWorld();
	if (!World) return;

	World->GetTimerManager().SetTimer(DashDurationTimerHandle, this, &UIncreaseMoveSpeed::EndBuff , _BuffDuration, false);
}

void UIncreaseMoveSpeed::SetUp()
{
	Super::SetUp();

	UWeaponComponent* WeaponComp = Cast<UWeaponComponent>(GetAttachParent());
	if (WeaponComp)
	{
		AFPSCharacter* Owenr = Cast<AFPSCharacter>(WeaponComp->GetOwnerCharacter());
		_OwnerMovingSpeed = Owenr->GetMovingSpeed();
	}
}

void UIncreaseMoveSpeed::SetOwnerSpeed(float speedamount)
{
	UWeaponComponent* WeaponComp = Cast<UWeaponComponent>(GetAttachParent());
	if (WeaponComp)
	{
		AFPSCharacter* Owenr = Cast<AFPSCharacter>(WeaponComp->GetOwnerCharacter());
		if (Owenr) {
			Owenr->GetCharacterMovement()->MaxWalkSpeed = speedamount;
		}
	}
}

void UIncreaseMoveSpeed::EndBuff()
{
	SetOwnerSpeed(_OwnerMovingSpeed);
}

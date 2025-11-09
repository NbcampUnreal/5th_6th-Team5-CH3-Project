// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Skill/InvincibilityAbility.h"
#include "Weapon/GunComponent.h"
#include "FPSCharacter.h"

void UInvincibilityAbility::Active()
{
	Super::Active();

	if (_isSkillCoolDown) return;
	_isSkillCoolDown = true;

	UGunComponent* GunComp = Cast<UGunComponent>(GetAttachParent());
	//GunComp->GetOwnerCharacter()->Setun
}

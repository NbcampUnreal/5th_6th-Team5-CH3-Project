// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Skill/UnlimitedAmmoAbility.h"
#include "Weapon/GunComponent.h"

void UUnlimitedAmmoAbility::GetAmmo()
{
	UGunComponent* GunComp = Cast<UGunComponent>(GetAttachParent());
	if (GunComp) {
		if (IsBuffActive)
		{
			GunComp->AddBullet(1);
		}
	}
}

void UUnlimitedAmmoAbility::SetUp()
{
	Super::SetUp();
	UGunComponent* GunComp = Cast<UGunComponent>(GetAttachParent());
	if (GunComp)
	{
		GunComp->OnDoAttack.AddUObject(this, &UUnlimitedAmmoAbility::GetAmmo);
	}
}

void UUnlimitedAmmoAbility::Active()
{
	Super::Active();

	if (_isSkillCoolDown) return;
	_isSkillCoolDown = true;
	IsBuffActive = true;

	UWorld* const World = GetWorld();
	if (!World) return;
	World->GetTimerManager().SetTimer(_BuffDurationTimerHandle, this, &UUnlimitedAmmoAbility::EndBuff, _BuffDuration, false);
}

void UUnlimitedAmmoAbility::EndBuff()
{
	IsBuffActive = false;
}

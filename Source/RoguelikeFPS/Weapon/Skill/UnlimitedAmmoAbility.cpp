// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Skill/UnlimitedAmmoAbility.h"
#include "Weapon/GunComponent.h"

void UUnlimitedAmmoAbility::GetAmmo(AProjectile* projectile)
{
	UGunComponent* GunComp = Cast<UGunComponent>(GetAttachParent());
	if(GunComp) GunComp->AddBullet(1);
}

void UUnlimitedAmmoAbility::SetUp()
{
	Super::SetUp();
	UGunComponent* GunComp = Cast<UGunComponent>(GetAttachParent());
	if (GunComp)
	{
		GunComp->ProjectileSpawn.AddUObject(this, &UUnlimitedAmmoAbility::GetAmmo);
	}
}

void UUnlimitedAmmoAbility::Active()
{
	Super::Active();

	if (_isSkillCoolDown) return;
	_isSkillCoolDown = true;
}

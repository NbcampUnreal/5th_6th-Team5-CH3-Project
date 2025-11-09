// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Skill/HeadshotGainAmmoAbility.h"
#include "Weapon/GunComponent.h"

void UHeadshotGainAmmoAbility::SetUp()
{
	Super::SetUp();

	UGunComponent* GunComp = Cast<UGunComponent>(GetAttachParent());
	if (GunComp)
	{
		GunComp->ProjectileSpawn.AddUObject(this, &UHeadshotGainAmmoAbility::Projectile_AddDynamic);
	}
}

void UHeadshotGainAmmoAbility::Projectile_AddDynamic(AProjectile* projectile)
{
	if (projectile)
	{
		projectile->OnHitHead.AddUObject(this, &UHeadshotGainAmmoAbility::GainBullet);
	}
}

void UHeadshotGainAmmoAbility::GainBullet()
{
	UGunComponent* GunComp = Cast<UGunComponent>(GetAttachParent());
	if (GunComp) {
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Blue, FString::Printf(TEXT("GainBullet")));
		GunComp->AddBullet(1);
	}
}

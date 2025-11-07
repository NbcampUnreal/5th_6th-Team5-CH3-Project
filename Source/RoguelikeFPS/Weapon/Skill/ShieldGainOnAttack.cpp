// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Skill/ShieldGainOnAttack.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/GunComponent.h"
#include "Weapon/Projectile.h"
#include "FPSCharacter.h"

void UShieldGainOnAttack::SetUp()
{
	UGunComponent* GunComp = Cast<UGunComponent>(GetAttachParent());
	if (GunComp)
	{

		GunComp->ProjectileSpawn.AddUObject(this, &UShieldGainOnAttack::Projectile_AddDynamic);
	}
}

void UShieldGainOnAttack::Projectile_AddDynamic(AProjectile* projectile)
{
	if (projectile)
	{
		projectile->OnDamagedEnemy.AddUObject(this, &UShieldGainOnAttack::GetShield);
	}
}

void UShieldGainOnAttack::GetShield(ACharacter* Victim)
{
	UGunComponent* GunComp = Cast<UGunComponent>(GetAttachParent());
	if (GunComp)
	{
		if (GunComp->GetOwnerCharacter())
		{
			AFPSCharacter* WeaponOwner = Cast<AFPSCharacter>(GunComp->GetOwnerCharacter());
			WeaponOwner->AddShield(_ShieldAmountGained);
			
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("Shield : %d"), WeaponOwner->GetShieldValue()));
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Skill/ChanceToStunAbility.h"
#include "Weapon/GunComponent.h"
#include "AI/Character/AIEnemyCharacter.h"


void UChanceToStunAbility::SetUp()
{
	Super::SetUp();

	UGunComponent* GunComp = Cast<UGunComponent>(GetAttachParent());
	if (GunComp) {
		GunComp->ProjectileSpawn.AddUObject(this, &UChanceToStunAbility::SetProjectile);
	}
}

void UChanceToStunAbility::SetProjectile(AProjectile* projectile)
{
	projectile->OnDamagedEnemy.AddUObject(this,&UChanceToStunAbility::ApplyStun);
}

void UChanceToStunAbility::ApplyStun(ACharacter* Victim)
{
	if (FMath::FRand() < _Chance)
	{
		if (Victim)
		{
			AAIEnemyCharacter* Enemy = Cast<AAIEnemyCharacter>(Victim);
			if (Enemy)
			{
				Enemy->ApplyStun(_Duration);
			}
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Skill/DamageRampUpAbility.h"
#include "Weapon/GunComponent.h"

void UDamageRampUpAbility::SetUp()
{
	Super::SetUp();

	UGunComponent* GunComp = Cast<UGunComponent>(GetAttachParent());
	if (GunComp) {
		GunComp->OnDoAttack.AddUObject(this, &UDamageRampUpAbility::GetStack);
		GunComp->ProjectileSpawn.AddUObject(this, &UDamageRampUpAbility::SetProjectileDamge);
	}
}

void UDamageRampUpAbility::Active()
{
	Super::Active();
}

void UDamageRampUpAbility::SetProjectileDamge(AProjectile* projectile)
{
	if (projectile)
	{
		projectile->AddDamage(_BonusDamagePerStack * _CurrentStack);
		GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Blue, FString::Printf(TEXT("AddDamage : %f"), projectile->GetDamage()));
	}
}

void UDamageRampUpAbility::GetStack()
{
	UWorld* const World = GetWorld();
	if (!World) return;
	World->GetTimerManager().SetTimer(_BuffDurationTimerHandle, this, &UDamageRampUpAbility::ResetStack, _BuffDuration, false);
	if(_CurrentStack + 1 < _MaxStack) _CurrentStack++;
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Green, FString::Printf(TEXT("GetStack : %d"), _CurrentStack));
}

void UDamageRampUpAbility::ResetStack()
{
	GEngine->AddOnScreenDebugMessage(3, 1.f, FColor::Blue, FString::Printf(TEXT("UDamageRampUpAbility : ResetStack")));
	_CurrentStack = 0;
}
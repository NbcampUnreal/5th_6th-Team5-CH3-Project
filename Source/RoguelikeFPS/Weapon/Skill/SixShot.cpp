// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Skill/SixShot.h"
#include "Weapon/GunComponent.h"

void USixShot::Active()
{
	Super::Active();

	if (_isSkillCoolDown) return;
	
	UWorld* World = GetWorld();
	if (!World) return;

	BurstCount = 0;
	_isSkillCoolDown = true;

	World->GetTimerManager().SetTimer(
		BurstTimerHandle,
		this,
		&USixShot::BurstTick,
		_AttackDelay,
		true // 반복
	);
}

void USixShot::BurstTick()
{
	if (GetAttachParent())
	{
		UGunComponent* GunComp = Cast<UGunComponent>(GetAttachParent());
		if (!IsValid(GunComp))
		{
			GetWorld()->GetTimerManager().ClearTimer(BurstTimerHandle);
			_isSkillCoolDown = false;
			return;
		}
		GunComp->SetDamage(_Damage);
		GunComp->Fire();
		BurstCount++;
		if (BurstCount >= 6)
		{
			// 끝났으면 타이머 정리
			GetWorld()->GetTimerManager().ClearTimer(BurstTimerHandle);
		}
	}
}

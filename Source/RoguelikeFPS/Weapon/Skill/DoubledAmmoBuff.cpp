// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/Skill/DoubledAmmoBuff.h"
#include "Weapon/GunComponent.h"

void UDoubledAmmoBuff::Active()
{
	Super::Active();

	if (_isSkillCoolDown) return;
	if (ApplyDoubleAmmoEffect(true))
	{
		_isSkillCoolDown = true;
		UWorld* const World = GetWorld();
		if (!World) return;
		World->GetTimerManager().SetTimer(_BuffTimerHandle, this, &UDoubledAmmoBuff::EndBuff, _BuffDuration, false);
	}
}

bool UDoubledAmmoBuff::ApplyDoubleAmmoEffect(bool bEnableDouble)
{
	UGunComponent* Guncomp = Cast<UGunComponent>(GetAttachParent());
	if (Guncomp)
	{
		int32 BulletCount = Guncomp->GetBulletCount();
		BulletCount *= bEnableDouble ? 2 : 0.5;
		Guncomp->SetBulletCount(BulletCount);
		return true;
	}

	return false;
}

void UDoubledAmmoBuff::EndBuff() {
	ApplyDoubleAmmoEffect(false);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Skill/QuickReloadBuff.h"
#include "Weapon/GunComponent.h"

void UQuickReloadBuff::Active()
{
	Super::Active();

	if (_isSkillCoolDown) return;

	UGunComponent* GunComp = Cast<UGunComponent>(GetAttachParent());
	if (GunComp)
	{
		GunComp->SetRealodTime(GunComp->GetRealodTime() * (1.0f - _ReloadTimeReductionRatio));
	}

	UWorld* const World = GetWorld();
	if (!World) return;
	World->GetTimerManager().SetTimer(_BuffDurationTimerHandle, this, &UQuickReloadBuff::EndBuff, _BuffDuration, false);
}

void UQuickReloadBuff::EndBuff()
{
	UGunComponent* GunComp = Cast<UGunComponent>(GetAttachParent());
	if (GunComp)
	{
		GunComp->SetRealodTime(GunComp->GetRealodTime() / (1.0f - _ReloadTimeReductionRatio));
	}
}
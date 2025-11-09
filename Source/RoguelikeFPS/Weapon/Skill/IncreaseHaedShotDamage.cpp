// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Skill/IncreaseHaedShotDamage.h"
#include "Weapon/GunComponent.h"

void UIncreaseHaedShotDamage::SetUp()
{
	Super::SetUp();

	UGunComponent* GunComp = Cast<UGunComponent>(GetAttachParent());
	if (GunComp)
	{
		GunComp->IncreaseHeadShotMultiplier(_HeadShotMultiplier);
	}
}

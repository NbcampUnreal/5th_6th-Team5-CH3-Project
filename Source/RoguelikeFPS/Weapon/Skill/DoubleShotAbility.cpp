// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Skill/DoubleShotAbility.h"
#include "Weapon/GunComponent.h"

void UDoubleShotAbility::SetUp()
{
	Super::SetUp();
	
	UGunComponent* GunComp = Cast<UGunComponent>(GetAttachParent());
	if (GunComp)
	{
		GunComp->SetAttackPoint(GunComp->GetAttackPoint() * 0.75f);
		GunComp->SetProjectilesPerShot(2);
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Blue, FString::Printf(TEXT("UDoubleShotAbility")));
	}
}

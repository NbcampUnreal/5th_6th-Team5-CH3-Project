// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Skill/ChargeableDash.h"
#include "Weapon/WeaponComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FPSCharacter.h"

UChargeableDash::UChargeableDash()
{
	_ChargedDashCount = _MaxDashCount;
}

void UChargeableDash::Active()
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("Active")));
	if (_ChargedDashCount > 0)
	{
		StartDash();
	}
}

void UChargeableDash::SetUp()
{
	Super::SetUp();
	//SetDashMapping();
}

void UChargeableDash::IncreaseDashCharge()
{
	if (_ChargedDashCount + 1 <= _MaxDashCount) {
		_ChargedDashCount++;
		GEngine->AddOnScreenDebugMessage(1, 1, FColor::Blue, FString::Printf(TEXT("_ChargedDashCount : %d"), _ChargedDashCount));
		UWorld* const World = GetWorld();
		if (!World) return;
		World->GetTimerManager().SetTimer(_SkillTimerHandle, this, &UChargeableDash::IncreaseDashCharge, _CoolDown, false);
		return;
	}
	_isSkillCoolDown = false;
}

void UChargeableDash::StartDash() {
	if (_IsDash) return;

	UWeaponComponent* WeaponComp = Cast<UWeaponComponent>(GetAttachParent());
	if (WeaponComp)
	{
		AFPSCharacter* Owenr = Cast<AFPSCharacter>(WeaponComp->GetOwnerCharacter());
		if (Owenr) {
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("StartDash")));
			Owenr->GetCharacterMovement()->MaxWalkSpeed = Owenr->GetDashSpeed() * _DashMultifly;
			
			_ChargedDashCount--;
			_IsDash = true;
			Owenr->SetIsDashing(_IsDash);

			UWorld* const World = GetWorld();
			if (!World) return;

			FTimerHandle DashTimerHandle;
			World->GetTimerManager().SetTimer(DashTimerHandle, this, &UChargeableDash::StopDash, 0.5f, false);

			if (!_isSkillCoolDown)//is Already charging
			{
				World->GetTimerManager().SetTimer(_SkillTimerHandle, this, &UChargeableDash::IncreaseDashCharge, _CoolDown, false);
				_isSkillCoolDown = true;
			}
		}
	}
}

void UChargeableDash::StopDash() {
	UWeaponComponent* WeaponComp = Cast<UWeaponComponent>(GetAttachParent());
	if (WeaponComp)
	{
		AFPSCharacter* Owenr = Cast<AFPSCharacter>(WeaponComp->GetOwnerCharacter());
		if (Owenr) {
			Owenr->GetCharacterMovement()->MaxWalkSpeed = Owenr->GetMovingSpeed();
			_IsDash = false;
			Owenr->SetIsDashing(_IsDash);
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("StopDash")));
		}
	}
}
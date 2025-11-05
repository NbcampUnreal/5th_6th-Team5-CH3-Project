// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Skill/ChargeableDash.h"
#include "Weapon/WeaponComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

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

void UChargeableDash::SetDashMapping()
{
	//PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &AFPSCharacter::StartDash);
	UWeaponComponent* WeaponComp = Cast<UWeaponComponent>(GetAttachParent());
	if (WeaponComp)
	{
		if (WeaponComp->GetCharacterEnhancedInputComponent()) {
			UInputComponent* IC = Cast<UInputComponent>(WeaponComp->GetCharacterEnhancedInputComponent());
			if (IC)
			{
				// Remove existing Dash action binding.
				const FName DashName = TEXT("Dash");
				for (int32 i = IC->GetNumActionBindings() - 1; i >= 0; --i)
				{
					if (IC->GetActionBinding(i).GetActionName() == DashName) IC->RemoveActionBinding(i);
				}

				//Add new Dash action binding.
				IC->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &UChargeableDash::Active);
			}
		}
	}
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
		ACharacter* Owenr = WeaponComp->GetOwnerCharacter();
		if (Owenr) {
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("StartDash")));
			Owenr->GetCharacterMovement()->MaxWalkSpeed *= _DashMultifly;
			_ChargedDashCount--;
			_IsDash = true;

			UWorld* const World = GetWorld();
			if (!World) return;			

			FTimerHandle DashTimerHandle;
			World->GetTimerManager().SetTimer(DashTimerHandle, this, &UChargeableDash::StopDash, 1.f, false);

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
		ACharacter* Owenr = WeaponComp->GetOwnerCharacter();
		if (Owenr) {
			Owenr->GetCharacterMovement()->MaxWalkSpeed /= _DashMultifly;
			_IsDash = false;
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("StopDash")));
		}
	}
}
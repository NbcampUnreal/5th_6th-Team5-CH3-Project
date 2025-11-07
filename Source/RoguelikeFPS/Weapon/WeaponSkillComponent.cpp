// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponSkillComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GunComponent.h"

// Sets default values for this component's properties
UWeaponSkillComponent::UWeaponSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UWeaponSkillComponent::Active()
{
	if (_isSkillCoolDown) return;
	SetCoolDownTimer();
	SetDamge();
}

void UWeaponSkillComponent::SetUp()
{
	SetActionMapping();
}

void UWeaponSkillComponent::SetActionMapping()
{
	if (_SkillAction)
	{
		UWeaponComponent* WeaponComp = Cast<UWeaponComponent>(GetAttachParent());
		if (WeaponComp)
		{
			if (WeaponComp->GetCharacterEnhancedInputComponent())
			{
				WeaponComp->GetCharacterEnhancedInputComponent()->BindAction(_SkillAction, ETriggerEvent::Triggered, this, &UWeaponSkillComponent::Active);
			}
		}
	}
}

// Called when the game starts
void UWeaponSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UWeaponSkillComponent::OnRegister()
{
	Super::OnRegister();
}

void UWeaponSkillComponent::OnAttachmentChanged()
{
	Super::OnAttachmentChanged();

	if (!IsRegistered() || !GetOwner()) return;

    UWeaponComponent* WeaponComp = Cast<UWeaponComponent>(GetAttachParent());

    if (WeaponComp)
    {
		
    }
	else {
		UE_LOG(LogTemp, Fatal, TEXT("AttachParent is not WeaponComponent"));
	}
}

void UWeaponSkillComponent::SetCoolDownTimer()
{
	UWorld* const World = GetWorld();
	if (!World) return;
	World->GetTimerManager().SetTimer(_SkillTimerHandle, FTimerDelegate::CreateLambda([this]()
		{
			_isSkillCoolDown = false;
		}) , _CoolDown, false);
}

void UWeaponSkillComponent::SetDamge()
{
	UGunComponent* GunComp = Cast<UGunComponent>(GetAttachParent());
	if (GunComp) _Damage = GunComp->ReturnDamage() * _AttackDamageMultiplier;
}


// Called every frame
void UWeaponSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


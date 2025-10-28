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
}

void UWeaponSkillComponent::SetActionMapping()
{
	UWeaponComponent* WeaponComp = Cast<UWeaponComponent>(GetAttachParent());
	if (WeaponComp)
	{
		if (WeaponComp->GetCharacterEnhancedInputComponent())
		{
			if (_SkillAction)
			{
				WeaponComp->GetCharacterEnhancedInputComponent()->BindAction(_SkillAction, ETriggerEvent::Triggered, this, &UWeaponSkillComponent::Active);
			}
		}
		else {
			UE_LOG(LogTemp, Fatal, TEXT("GetCharacterEnhancedInputComponent() is null"));
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

	if (_SkillAction)
	{

	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("_SkillAction is null"));
	}
}

void UWeaponSkillComponent::SetCoolDownTimer()
{
	FTimerDelegate Delegate = FTimerDelegate::CreateLambda([this]()
		{
			_isSkillCoolDown = false;
		});

	UWorld* const World = GetWorld();
	World->GetTimerManager().SetTimer(_SkillTimerHandle, Delegate, _CoolDown, false);
}


// Called every frame
void UWeaponSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


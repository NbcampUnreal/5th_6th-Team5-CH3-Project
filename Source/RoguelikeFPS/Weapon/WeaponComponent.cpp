// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponComponent.h"
#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PickUpComponent.h"


UWeaponComponent::UWeaponComponent()
{
	
}

void UWeaponComponent::DoAttack()
{

}

void UWeaponComponent::AttachWeapon(ACharacter* TargetCharacter)
{
	_Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (_Character == nullptr || _Character->GetInstanceComponents().FindItemByClass<UWeaponComponent>())
	{
		return;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	GetOwner()->AttachToComponent(_Character->GetMesh(), AttachmentRules, FName(TEXT("GripPoint")));
	//AttachToComponent(_Character->GetMesh(), AttachmentRules, FName(TEXT("GripPoint")));

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(_Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(_AttackMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Attack
			EnhancedInputComponent->BindAction(_AttackAction, ETriggerEvent::Triggered, this, &UWeaponComponent::DoAttack);
			if (_SkillComponent) _SkillComponent->SetActionMapping();
		}
	}

	//return true;
}

void UWeaponComponent::ActiveSkill()
{
	if (_SkillComponent)
	{
		_SkillComponent->Active();
	}
}

UEnhancedInputComponent* UWeaponComponent::GetCharacterEnhancedInputComponent()
{
	if (_Character)
	{
		APlayerController* PlayerController = Cast<APlayerController>(_Character->GetController());
		if (PlayerController)
		{
			UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
			if (EnhancedInputComponent)
			{
				return EnhancedInputComponent;
			}
		}
	}

	return nullptr;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	//Check PickUpCompoent Attached
	//if PickUpCompoent is exsit. Do AddDynamic
	if (GetOwner() != nullptr)
	{
		UPickUpComponent* PickUp = GetOwner()->FindComponentByClass<UPickUpComponent>();
		if (PickUp)
		{
			PickUp->OnPickUp.AddDynamic(this, &UWeaponComponent::AttachWeapon);
			UE_LOG(LogTemp, Warning, TEXT("AddDynamic"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Can't find UPickUpComponent"));
		}
	}

	if (_TSubSkillComponent)
	{
		_SkillComponent = NewObject<UWeaponSkillComponent>(GetOwner(), _TSubSkillComponent);
		if (_SkillComponent)
		{
			_SkillComponent->RegisterComponent(); // 반드시 등록해야 월드에서 동작
			_SkillComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("SkillComponent is null"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("TSubSkillComponent is null"));
	}
}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// ensure we have a character owner
	if (_Character != nullptr)
	{
		// remove the input mapping context from the Player Controller
		if (APlayerController* PlayerController = Cast<APlayerController>(_Character->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(_AttackMappingContext);
			}
		}
	}

	Super::EndPlay(EndPlayReason);
}



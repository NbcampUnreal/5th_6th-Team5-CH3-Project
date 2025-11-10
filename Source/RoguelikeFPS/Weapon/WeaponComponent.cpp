// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponComponent.h"
#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PickUpComponent.h"
#include "FPSPlayerController.h"
#include "FPSCharacter.h"


UWeaponComponent::UWeaponComponent()
{
	SetCollisionProfileName("NoCollision");
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetGenerateOverlapEvents(false);
}

void UWeaponComponent::DoAttack()
{
	OnDoAttack.Broadcast();
}

void UWeaponComponent::AttachWeapon(ACharacter* TargetCharacter)
{
	_Character = TargetCharacter;


	// Check that the character is valid, and has no weapon component yet
	if (_Character == nullptr || _Character->GetInstanceComponents().FindItemByClass<UWeaponComponent>())
	{
		return;
	}

	if (TargetCharacter)
	{
		AFPSCharacter* fpscharacter = Cast<AFPSCharacter>(TargetCharacter);
		fpscharacter->CurrentWeapon = this;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);
	this->AttachToComponent(_Character->GetMesh(), AttachmentRules, WeaponSocketName);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(_Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(_AttackMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Attack
			EnhancedInputComponent->BindAction(_AttackAction, ETriggerEvent::Triggered, this, &UWeaponComponent::DoAttack);
			SetUpWeaponSkills();
		}
	}

	if (_TSubAnimInstance)
	{
		_Character->GetMesh()->SetAnimInstanceClass(_TSubAnimInstance);
	}
}

UEnhancedInputComponent* UWeaponComponent::GetCharacterEnhancedInputComponent()
{
	if (_Character)
	{
		AFPSPlayerController* PlayerController = Cast<AFPSPlayerController>(_Character->GetController());
		if (PlayerController)
		{
			UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
			if (EnhancedInputComponent)
			{
				return EnhancedInputComponent;
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent:: PlayerController is null"));
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("UWeaponComponent:: PlayerController is null"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("UWeaponComponent:: _Character is null"));
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
		}
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

void UWeaponComponent::OnChildAttached(USceneComponent* ChildComponent)
{
	Super::OnChildAttached(ChildComponent);
	UWeaponSkillComponent* Skill = Cast<UWeaponSkillComponent>(ChildComponent);
	if (Skill && _Character)
	{
		Skill->SetUp();
		return;
	}
}

void UWeaponComponent::SetUpWeaponSkills()
{
	TArray<UWeaponSkillComponent*> SkillComps;

	// 현재 직속 자식들 중 UWeaponSkillComponent만 골라서 복사
	const TArray<USceneComponent*>& Children = GetAttachChildren();
	for (USceneComponent* Child : Children)
	{
		if (UWeaponSkillComponent* Skill = Cast<UWeaponSkillComponent>(Child))
		{
			SkillComps.Add(Skill);
		}
	}

	// 복사한 배열을 안전하게 순회하며 SetUp 호출
	for (UWeaponSkillComponent* Skill : SkillComps)
	{
		if (IsValid(Skill))
		{
			Skill->SetUp();
		}
	}
}

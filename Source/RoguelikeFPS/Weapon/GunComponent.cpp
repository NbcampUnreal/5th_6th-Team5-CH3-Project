// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GunComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SCS_Node.h"

UGunComponent::UGunComponent()
{
	CanAttack = true;
}

UGunComponent::~UGunComponent()
{
	
}

void UGunComponent::DoAttack()
{
	if (!CanAttack) return;
	if (_Character == nullptr || _Character->GetController() == nullptr)
	{
		return;
	}

	SetDamage();

	for (size_t count = 0; count < _Status.ProjectilesPerShot; count++)
	{
		Fire();
	}

	CurrentBulletCount--;
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, FString::Printf(TEXT("CurrentBulletCount : %d"), CurrentBulletCount));

	CanAttack = false;

	FTimerDelegate Delegate = FTimerDelegate::CreateLambda([this]()
		{
			CanAttack = true;
		});

	UWorld* const World = GetWorld();
	World->GetTimerManager().SetTimer(_GunTimerHandle, Delegate, _Status.AttackDelay, false);

	// Try and play the sound if specified b 
	if (_AttackSound != nullptr) {
		UGameplayStatics::PlaySoundAtLocation(this, _AttackSound, _Character->GetActorLocation());
	}

	// Try and play a firing animation if specified
	if (_AttackAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = _Character->GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(_AttackAnimation, 1.f);
	}
}

void UGunComponent::BeginPlay()
{
	Super::BeginPlay();
	InitProjectile();
	CurrentBulletCount = _Status.MaxBulletCount;
}


void UGunComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	Super::EndPlay(EndPlayReason);
}

void UGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGunComponent::SetDamage()
{
	if (_Templete)
	{

	}
}

void UGunComponent::Fire()
{
	if (!(CurrentBulletCount > 0)) {
		ReloadBullet();
		return;
	}

	if (_ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(_Character->GetController());
			//const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			//const FVector SpawnLocation = this->GetSocketLocation(FName("Muzzle"));

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ActorSpawnParams.Template = _Templete;

			// Spawn the projectile at the muzzle
			World->SpawnActor<AActor>(_ProjectileClass, this->GetSocketLocation(FName("Muzzle")), PlayerController->PlayerCameraManager->GetCameraRotation(), ActorSpawnParams);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("_ProjectileClass nullptr"));
	}
}

void UGunComponent::InitProjectile()
{
	if (_ProjectileClass)
	{
		//Templete
		AProjectile* TempActor = _ProjectileClass->GetDefaultObject<AProjectile>();
		TempActor->SetMovement(_Status.ProjectileSpeed);
		_Templete = TempActor;
	}
}

void UGunComponent::ReloadBullet()
{
	CurrentBulletCount = _Status.MaxBulletCount;
	GEngine->AddOnScreenDebugMessage(2, 0.5f, FColor::Red, FString::Printf(TEXT("ReloadBullet")));
}

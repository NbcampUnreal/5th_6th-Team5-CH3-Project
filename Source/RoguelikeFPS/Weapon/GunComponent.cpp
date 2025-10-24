// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GunComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectileComponent.h"
#include "Engine/SCS_Node.h"

UGunComponent::UGunComponent()
{
	
}

UGunComponent::~UGunComponent()
{
	
}

void UGunComponent::DoAttack()
{
	if (_Character == nullptr || _Character->GetController() == nullptr)
	{
		return;
	}

	SetDamage();

	Fire();

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

	//if (!Check_ProjectileHaveComponent())
	//{
	//	UE_LOG(LogTemp, Fatal, TEXT("Gun's Projectile does not have Component"));
	//}

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
	/*if (_Projectile)
	{
		_Projectile->Damage = _Status.AttackPoint;
	}*/
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
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			const FVector SpawnLocation = this->GetSocketLocation(FName("Muzzle"));

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// Spawn the projectile at the muzzle
			World->SpawnActor<AActor>(_ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

			CurrentBulletCount--;
			GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, FString::Printf(TEXT("CurrentBulletCount : %d"), CurrentBulletCount));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("_ProjectileClass nullptr"));
	}
}

void UGunComponent::InitProjectile()
{
	//
}

void UGunComponent::ReloadBullet()
{
	CurrentBulletCount = _Status.MaxBulletCount;
	GEngine->AddOnScreenDebugMessage(2, 0.5f, FColor::Red, FString::Printf(TEXT("ReloadBullet")));
}

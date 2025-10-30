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
	if (_Character == nullptr || _Character->GetController() == nullptr) return;

	if (!(CurrentBulletCount > 0)) {
		ReloadBullet();
		return;
	}

	for (size_t count = 0; count < _Status.ProjectilesPerShot; count++)
	{
		Fire();
	}

	CurrentBulletCount--;
	//GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Green, FString::Printf(TEXT("CurrentBulletCount : %d"), CurrentBulletCount));

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

void UGunComponent::Fire()
{
	if (_ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			//const FRotator SpawnRotation = CalculateSapwnRotaion();
			const FRotator SpawnRotation = CalculateSapwnRotaion();
			const FVector SpawnLocation = this->GetSocketLocation(FName("Muzzle"));

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// Spawn the projectile at the muzzle
			AProjectile* SpawnProjectile = World->SpawnActorDeferred<AProjectile>(_ProjectileClass, FTransform(SpawnRotation, SpawnLocation));
			if (!IsValid(SpawnProjectile)) return;
			InitSpawnProjectile(SpawnProjectile);
			ProjectileSpawn.Broadcast(SpawnProjectile);
			UGameplayStatics::FinishSpawningActor(SpawnProjectile, FTransform(SpawnRotation, SpawnLocation));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("_ProjectileClass nullptr"));
	}
}

FRotator UGunComponent::CalculateSapwnRotaion()
{
	if (_Character)
	{
		APlayerController* PlayerController = Cast<APlayerController>(_Character->GetController());
		if (PlayerController)
		{
			FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();


			float AccuracyChance = FMath::FRand();
			if (AccuracyChance < _Status.Accuracy) {

			}
			else {
				float ConeHalfAngleRad = FMath::DegreesToRadians(_Status.Spread_Angle);
				FVector Forward = SpawnRotation.Vector();
				FVector ShotDir = FMath::VRandCone(Forward, ConeHalfAngleRad);
				FRotator SpawnRot = ShotDir.Rotation();
				SpawnRotation = SpawnRot;
			}

			return SpawnRotation;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("PlayerController is nullptr"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("_Character is nullptr"));
	}

	return FRotator();
}

void UGunComponent::InitSpawnProjectile(AProjectile* proejectile)
{
	if (proejectile)
	{
		proejectile->SetMovementSpeed(_Status.ProjectileSpeed);
		proejectile->SetInstigator(_Character);
		proejectile->SetWeapon(GetOwner());
		proejectile->SetDamage(_Status.AttackPoint);
	}
}

void UGunComponent::ReloadBullet()
{
	CurrentBulletCount = _Status.MaxBulletCount;
	GEngine->AddOnScreenDebugMessage(2, 0.5f, FColor::Red, FString::Printf(TEXT("ReloadBullet")));
}

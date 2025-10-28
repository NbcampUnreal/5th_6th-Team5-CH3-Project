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

	SetDamage();

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
		float Damage = _Status.AttackPoint;
		if (FMath::FRand() < _Status.CriticalChance) Damage *= _Status.CriticalMultiplier;
		_Templete->Damage = Damage;
	}
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
			ActorSpawnParams.Template = _Templete;

			// Spawn the projectile at the muzzle
			World->SpawnActor<AActor>(_ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("_ProjectileClass nullptr"));
	}
}

void UGunComponent::SetDamage(float Damage)
{
	_Status.AttackPoint += Damage;
	SetDamage();
	_Status.AttackPoint -= Damage;
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GunComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SCS_Node.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"

UGunComponent::UGunComponent()
{
	CanAttack = true;
	//_WeaponSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("WeaponSpringArm"));
	//_WeaponSpringArm->TargetArmLength = 0.f;
	//_WeaponSpringArm->bDoCollisionTest = false;

	//_WeaponCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("WeaponCamera"));
	//_WeaponCamera->SetupAttachment(_WeaponSpringArm);
}

UGunComponent::~UGunComponent()
{
	
}

void UGunComponent::DoAttack()
{
	if (!CanAttack) return;
	if (_Character == nullptr || _Character->GetController() == nullptr) return;

	if ((CurrentBulletCount <= 0)) {
		if(_IsReloading) return;
		UWorld* World = GetWorld();
		GetWorld()->GetTimerManager().SetTimer(_ReloadTimerHandle, this, &UGunComponent::ReloadBullet, _Status.ReloadTime, false);
		_IsReloading = true;
		return;
	}

	Fire();
	
	OnDoAttack.Broadcast();
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

void UGunComponent::OnRegister()
{
	Super::OnRegister();
}

void UGunComponent::AttachWeapon(ACharacter* TargetCharacter)
{
	Super::AttachWeapon(TargetCharacter);

	//if (APlayerController* PlayerController = Cast<APlayerController>(_Character->GetController()))
	//{
	//	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
	//	{
	//		// Attack
	//		EnhancedInputComponent->BindAction(_ZoomAction, ETriggerEvent::Triggered, this, &UGunComponent::ZoomIn);
	//		//EnhancedInputComponent->BindAction(_ZoomAction, ETriggerEvent::, this, &UGunComponent::ZoomOut);
	//	}
	//}
}

void UGunComponent::Fire()
{
	for (size_t count = 0; count < _Status.ProjectilesPerShot; count++)
	{
		SpawnProejectile();
	}

	CurrentBulletCount--;
	CanAttack = false;


	auto* it = GetAttachParent();
	UWorld* World = it->GetWorld();
	World->GetTimerManager().SetTimer(_GunTimerHandle, [this]()
		{
			CanAttack = true;
		}, _Status.AttackDelay, false);

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

float UGunComponent::ReturnDamage()
{
	return _Status.AttackPoint;
}

void UGunComponent::AddBullet(int32 amount)
{
	if (amount > 0) CurrentBulletCount += amount;
}

void UGunComponent::SetBulletCount(int32 count)
{
	if (count >= 0) CurrentBulletCount = count;
}

void UGunComponent::SetRealodTime(float time)
{
	if (time >= 0) _Status.ReloadTime = time;
}

void UGunComponent::SetProjectilesPerShot(int32 count)
{
	_Status.ProjectilesPerShot = count < 1 ? 1 : count;
}

void UGunComponent::SetAttackPoint(float attackpoint)
{
	if (attackpoint >= 0) _Status.AttackPoint = attackpoint;
}

void UGunComponent::IncreaseHeadShotMultiplier(float value)
{
	if (value > 0) _Status.HeadShotMultiplier += value;
}

void UGunComponent::SetData(UGunDataAsset data)
{
	data.GunData.WeaponSocketName = WeaponSocketName;
	data.GunData._AttackAction = _AttackAction;
	data.GunData._AttackAnimation = _AttackAnimation;
	data.GunData._AttackMappingContext = _AttackMappingContext;
	data.GunData._AttackSound = _AttackSound;
	data.GunData._ProjectileClass = _ProjectileClass;
	data.GunData._TSubAnimInstance = _TSubAnimInstance;

	// 현재 직속 자식들 중 UWeaponSkillComponent만 골라서 복사
	const TArray<USceneComponent*>& Children = GetAttachChildren();
	for (USceneComponent* Child : Children)
	{
		if (UWeaponSkillComponent* Skill = Cast<UWeaponSkillComponent>(Child))
		{
			UClass* SkillClass = Skill->GetClass();
			if (SkillClass && SkillClass->IsChildOf(UWeaponSkillComponent::StaticClass()))
			{
				data.Skills.Add(SkillClass);
			}
		}
	}
}

void UGunComponent::Assign(UGunDataAsset data)
{
	WeaponSocketName = data.GunData.WeaponSocketName;
	_AttackAction = data.GunData._AttackAction;
	_AttackAnimation = data.GunData._AttackAnimation;
	_AttackMappingContext = data.GunData._AttackMappingContext;
	_AttackSound = data.GunData._AttackSound;
	_TSubAnimInstance = data.GunData._TSubAnimInstance;

	for (TSubclassOf<UWeaponSkillComponent> SkillClass : data.Skills)
	{
		if (!SkillClass) continue;
		UWeaponSkillComponent* NewSkill = NewObject<UWeaponSkillComponent>(_Character, SkillClass);
		NewSkill->SetupAttachment(_Character->GetMesh());
		NewSkill->RegisterComponent();
		NewSkill->SetUp();
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
			PlayerController->PlayerCameraManager->SetFOV(100.0f);

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
		proejectile->SetDamage(_Status.AttackPoint);
		proejectile->SetHeadShotMultiplier(_Status.HeadShotMultiplier);
	}
}

void UGunComponent::ReloadBullet()
{
	CurrentBulletCount = _Status.MaxBulletCount;
	_IsReloading = false;
	GEngine->AddOnScreenDebugMessage(2, 0.5f, FColor::Red, FString::Printf(TEXT("ReloadBullet")));
}

void UGunComponent::SpawnProejectile()
{
	if (_ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			const FRotator SpawnRotation = CalculateSapwnRotaion();
			const FVector SpawnLocation = this->GetSocketLocation(FName("Muzzle"));

			// Spawn the projectile at the muzzle
			AProjectile* SpawnProjectile = World->SpawnActorDeferred<AProjectile>(_ProjectileClass, FTransform(SpawnRotation, SpawnLocation), nullptr, GetOwnerCharacter(), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
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

void UGunComponent::ZoomIn() {

}

void UGunComponent::ZoomOut() {

}

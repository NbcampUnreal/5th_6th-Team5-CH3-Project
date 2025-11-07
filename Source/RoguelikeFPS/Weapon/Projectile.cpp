// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Weapon/WeaponComponent.h"
#include "Components/SphereComponent.h"
#include "FPSCharacter.h"
#include "AI/Character/AIEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	_Collision->InitSphereRadius(5.0f);
	_Collision->SetCollisionProfileName("Projectile");
	
	_Collision->SetGenerateOverlapEvents(false);
	_Collision->SetNotifyRigidBodyCollision(true);
	_Collision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);
	_Collision->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);


	_Collision->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	_Collision->CanCharacterStepUpOn = ECB_No;

	RootComponent = _Collision;

	_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	_ProjectileMovement->UpdatedComponent = _Collision;
	_ProjectileMovement->InitialSpeed = 0.f;
	_ProjectileMovement->MaxSpeed = 0.f;
	_ProjectileMovement->bRotationFollowsVelocity = true;
	_ProjectileMovement->bShouldBounce = true;
	_ProjectileMovement->ProjectileGravityScale = 0.f;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green , FString::Printf(TEXT("BeginPlay")));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *GetActorLocation().ToString()));
}

void AProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//OnDestroyed.Broadcast();

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (OtherComp->IsA(UWeaponComponent::StaticClass())) return;
	//if (!IsValid(_Instigator)) return;
	//if (_Instigator == OtherActor) return;
	//
	//ACharacter* Victim = Cast<ACharacter>(OtherActor);	

	//if (!Victim)
	//{
	//	Destroy();
	//	return;
	//}

	//ACharacter* Attacker = Cast<ACharacter>(_Instigator);

	//AActor* ActualDamageCauser = this;
	//AController* AttackerController = nullptr;
	//if (IsValid(Attacker))
	//{
	//	AttackerController = Attacker->GetController();
	//}

	//const FVector ShotDir = (OtherActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	//UGameplayStatics::ApplyDamage(Victim, _Damage, AttackerController, ActualDamageCauser, UDamageType::StaticClass());
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("ApplyDamage : %d"), _Damage));
	//OnDamagedEnemy.Broadcast(Victim);
	//Destroy();
}


void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherComp->IsA(UWeaponComponent::StaticClass())) return;
	if (!IsValid(_Instigator)) return;
	if (_Instigator == OtherActor) return;
	
	ACharacter* Victim = Cast<ACharacter>(OtherActor);	

	if (!Victim)
	{
		Destroy();
		return;
	}

	ACharacter* Attacker = Cast<ACharacter>(_Instigator);

	AActor* ActualDamageCauser = this;
	AController* AttackerController = nullptr;
	if (IsValid(Attacker))
	{
		AttackerController = Attacker->GetController();
	}

	const FVector ShotDir = (OtherActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	UGameplayStatics::ApplyPointDamage(OtherActor, _Damage, ShotDir, Hit, AttackerController, this, UDamageType::StaticClass()
	);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("ApplyDamage : %d"), _Damage));
	OnDamagedEnemy.Broadcast(Victim);
	Destroy();
}

void AProjectile::SetMovementSpeed(float speed)
{
	if (speed < 0) return;
	_ProjectileMovement->MaxSpeed = speed;
	_ProjectileMovement->InitialSpeed = speed;

	FVector CurrentDir = GetActorForwardVector();
	if (!_ProjectileMovement->Velocity.IsNearlyZero())
	{
		CurrentDir = _ProjectileMovement->Velocity.GetSafeNormal();
	}
	_ProjectileMovement->Velocity = CurrentDir * speed;
}

void AProjectile::SetInstigator(AActor* instigator)
{
	if (instigator) _Instigator = instigator;
}

void AProjectile::SetDamage(float damage)
{
	if (0 < damage)
	{
		_Damage = damage;
	}
}


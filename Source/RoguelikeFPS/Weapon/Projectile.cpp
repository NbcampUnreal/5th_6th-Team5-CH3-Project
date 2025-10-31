// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Weapon/WeaponComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	_Collision->InitSphereRadius(5.0f);
	_Collision->SetCollisionProfileName("Projectile");
	_Collision->SetGenerateOverlapEvents(true);
	_Collision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);
	//_Collision->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

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
	//if (!OtherActor || !OtherComp) return;
	//if (OtherActor == GetOwner()) return; // 소유자 자신 무시
	//if (OtherComp->IsA(UWeaponComponent::StaticClass())) return;
	//
	//if (IsValid(_Instigator))
	//{
	//	if (_Instigator == OtherActor)
	//	{
	//		/*GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Red, FString::Printf(TEXT("Instigato Name : %s"), *_Instigator->GetName()));
	//		GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Blue, FString::Printf(TEXT("OtherActor Name : %s"), *OtherActor->GetName()));*/
	//	}
	//	else {
	//		Destroy();
	//	}
	//}
	if (OtherComp->IsA(UWeaponComponent::StaticClass())) return;
	if (!IsValid(_Instigator)) return;
	if (_Instigator != OtherActor) Destroy();	
}


void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Destroy();
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


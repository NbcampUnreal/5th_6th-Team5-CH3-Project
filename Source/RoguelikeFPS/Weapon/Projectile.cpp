// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	_Collision->InitSphereRadius(5.0f);
	_Collision->SetCollisionProfileName("Projectile");
	_Collision->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	_Collision->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	_Collision->CanCharacterStepUpOn = ECB_No;

	RootComponent = _Collision;

	_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	_ProjectileMovement->UpdatedComponent = _Collision;
	_ProjectileMovement->InitialSpeed = 3000.f;
	_ProjectileMovement->MaxSpeed = 3000.f;
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
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != GetOwner()) && (OtherComp != nullptr))
	{
		//OtherComp->AddImpulseAtLocation(GetOwner()->GetVelocity() * 100.0f, GetOwner()->GetActorLocation());

		GetOwner()->Destroy();
	}
}

void AProjectile::SetMovement(float speed)
{
	if (speed < 0) return;
	_ProjectileMovement->InitialSpeed = speed;
	_ProjectileMovement->MaxSpeed = speed;
}


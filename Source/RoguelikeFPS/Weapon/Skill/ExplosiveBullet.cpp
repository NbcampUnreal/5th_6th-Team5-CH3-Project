// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Skill/ExplosiveBullet.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/GunComponent.h"

void UExplosiveBullet::SetUp()
{
	Super::SetUp();

	UGunComponent* GunComp = Cast<UGunComponent>(GetAttachParent());
	if (GunComp)
	{
		GunComp->ProjectileSpawn.AddUObject(this, &UExplosiveBullet::Projectile_AddDynamic);
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("SetUp : AddDynamic")));
	}
}

void UExplosiveBullet::Projectile_AddDynamic(AProjectile* projectile)
{
	if (projectile)
	{
		projectile->OnDestroyed.AddDynamic(this, &UExplosiveBullet::Spawn_Explosion);
	}
}

void UExplosiveBullet::Spawn_Explosion(AActor* DestroyedActor)
{
	UpdateDamge();

	if (_Explosion)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			const FRotator SpawnRotation = DestroyedActor->GetActorRotation();
			const FVector SpawnLocation = DestroyedActor->GetActorLocation();

			AExplosiveActor* ExplosiveActor = World->SpawnActorDeferred<AExplosiveActor>(_Explosion, FTransform(SpawnRotation, SpawnLocation));
			if (!IsValid(ExplosiveActor)) return;
			ExplosiveActor->_Damage = this->_Damage;
			UGameplayStatics::FinishSpawningActor(ExplosiveActor, FTransform(SpawnRotation, SpawnLocation));
		}
	}
}

void UExplosiveBullet::UpdateDamge()
{
	UGunComponent* GunComp = Cast<UGunComponent>(GetAttachParent());
	if (GunComp) _Damage = GunComp->ReturnDamage();
}

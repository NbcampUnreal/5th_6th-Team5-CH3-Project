// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Skill/ExplosiveActor.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AExplosiveActor::AExplosiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	_Collision->SetCollisionProfileName("Projectile");
	SetRootComponent(_Collision);

	_Collision->OnComponentBeginOverlap.AddDynamic(this, &AExplosiveActor::OnSphereBeginOverlap);
	InitialLifeSpan = 0.5f;
}

// Called when the game starts or when spawned
void AExplosiveActor::BeginPlay()
{
	Super::BeginPlay();

	UNiagaraComponent* NiComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Explosion, GetActorLocation(), GetActorRotation());
	NiComp->SetNiagaraVariableFloat(TEXT("User.Range"), _Collision->GetScaledSphereRadius() * 2);
	float count = (_Collision->GetScaledSphereRadius() / 500) * 100;
	NiComp->SetNiagaraVariableInt(TEXT("User.SpawnCount"), count > 36 ? count : 36);

	//Spawn Particle
	if (Explosion) UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Explosion, GetActorLocation(), GetActorRotation());
}

// Called every frame
void AExplosiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosiveActor::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character)
	{  
		
	}
}
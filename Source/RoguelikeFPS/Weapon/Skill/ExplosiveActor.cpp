// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Skill/ExplosiveActor.h"
#include "GameFramework/Character.h"

// Sets default values
AExplosiveActor::AExplosiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(_Collision);

	_Collision->OnComponentBeginOverlap.AddDynamic(this, &AExplosiveActor::OnSphereBeginOverlap);

	InitialLifeSpan = 0.1f;
}

// Called when the game starts or when spawned
void AExplosiveActor::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("%s"), *GetActorLocation().ToString()));
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
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("OnSphereBeginOverlap")));
	}
}
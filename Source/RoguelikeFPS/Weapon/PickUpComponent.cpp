// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PickUpComponent.h"
//#include ""

UPickUpComponent::UPickUpComponent()
{
	SphereRadius = 32.f;
}

void UPickUpComponent::BeginPlay() {
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UPickUpComponent::OnSphereBeginOverlap);
}

void UPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Character = Cast<APlayerCharacter>(OtherActor);
	if (Character)
	{
		OnPickUp.Broadcast(Character);

		OnComponentBeginOverlap.RemoveAll(this);
	}
}

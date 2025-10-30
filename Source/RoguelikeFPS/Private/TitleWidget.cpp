// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidget.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"	
#include "GameFramework/PlayerController.h"
#include "Components/Button.h"

bool UTitleWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (GameStartButton)
	{
		GameStartButton->OnClicked.AddDynamic(this, &UTitleWidget::OnGameStartClicked);
	}

	if (OptionButton)
	{
		OptionButton->OnClicked.AddDynamic(this, &UTitleWidget::OnOptionClicked);
	}

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UTitleWidget::OnExitClicked);
	}
	return true;
}
void UTitleWidget::OnGameStartClicked()
{
    if (UWorld* World = GetWorld())
	{
		//위젯 제거
		RemoveFromParent();

		OnStartButtonClicked.Broadcast();
	}
}
void UTitleWidget::OnOptionClicked()
{
	//옵션 관련해서는 만든 것 이 없으니까 일단 이렇게만 작성
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Option Menu Placeholder"));
}
void UTitleWidget::OnExitClicked()
{
	if (UWorld* World = GetWorld())
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
		UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, true);
	}
}


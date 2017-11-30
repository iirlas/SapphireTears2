// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Engine.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Builder.h"

AMyPlayerController::AMyPlayerController()
	:Super()
{
	myPawns.Init(nullptr, 2);
}

void AMyPlayerController::BeginPlay/*_Implementation*/()
{
	Super::BeginPlay();
	
	myPawns[0] = GetPawn();
	myPawns[1] = GetWorld()->SpawnActor<ABuilder>(myPawns[0]->GetTransform().GetLocation(), FRotator::ZeroRotator);
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("SwitchMode", IE_Pressed, this, &AMyPlayerController::OnSwitchMode);
}

void AMyPlayerController::OnSwitchMode_Implementation()
{
	UnPossess();
	myPawns.Swap(0, 1);
	Possess(myPawns[0]);
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SAPPHIRE2_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

	TArray<APawn*> myPawns;

public:
	AMyPlayerController ();

	//UFUNCTION(Client, Reliable)
	void BeginPlay ();

	virtual void SetupInputComponent();

	UFUNCTION(Client, Reliable)
	void OnSwitchMode ();
};

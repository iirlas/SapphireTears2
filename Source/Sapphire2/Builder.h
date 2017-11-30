// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Builder.generated.h"

UCLASS()
class SAPPHIRE2_API ABuilder : public APawn
{
	GENERATED_BODY()

	//UPROPERTY(Replicated)
	UInstancedStaticMeshComponent* myISMC;

	UPROPERTY(EditAnywhere)
	class UStaticMesh* myMesh;

	class UStaticMeshComponent* myPlacement;

	class UCameraComponent* myCamera;

	class UPawnMovementComponent* myMovement;

	UPROPERTY(EditAnywhere)
	float myRange = 1000.f;

public:
	// Sets default values for this pawn's properties
	ABuilder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(NetMulticast, Reliable)
	void OnFire ();

	void MoveForward(float value);
	void MoveRight(float value);
	void MoveUp(float value);

private:
	FVector GetPlacementLocation (FHitResult& hit_result);

	static FVector Align (FVector location, const FVector& size);
};

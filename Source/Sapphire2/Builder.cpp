// Fill out your copyright notice in the Description page of Project Settings.

#include "Builder.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"

// Sets default values
ABuilder::ABuilder()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> class_finder(TEXT("/Game/Geometry/Meshes/1M_Cube"));
	myMesh = class_finder.Object;

	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// Create Root Component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	// Create Floating Pawn Movement Component for spectator-like movement
	myMovement = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(TEXT("Movement"));
	myMovement->SetUpdatedComponent(RootComponent);
	// Create Camera
	myCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	myCamera->SetupAttachment(RootComponent);
	myCamera->bUsePawnControlRotation = true;
	// Create Static mesh to visualize placement
	myPlacement = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Placement"));
	myPlacement->bAbsoluteLocation = true;
	myPlacement->SetOnlyOwnerSee(true); // Only visible to this actor
	// Create 'Procedural' mesh component
	myISMC = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ISM"));
	myISMC->bAbsoluteLocation = true;
}

// Called when the game starts or when spawned
void ABuilder::BeginPlay()
{
	Super::BeginPlay();
	myPlacement->SetStaticMesh(myMesh);
	myPlacement->SetCollisionProfileName(TEXT("NoCollision"));
	myISMC->SetStaticMesh(myMesh);
}

void ABuilder::OnConstruction(const FTransform & Transform)
{
}

// Called every frame
void ABuilder::Tick(float delta_time)
{
	Super::Tick(delta_time);
	if (Controller)
	{
		FHitResult hit_result;
		FVector location = GetPlacementLocation(hit_result);
		myPlacement->SetWorldLocation(location);
	}
}

// Called to bind functionality to input
void ABuilder::SetupPlayerInputComponent(UInputComponent* player_input)
{
	Super::SetupPlayerInputComponent(player_input);

	player_input->BindAction("Fire", IE_Pressed, this, &ABuilder::OnFire);
	player_input->BindAxis("MoveForward", this, &ABuilder::MoveForward);
	player_input->BindAxis("MoveRight", this, &ABuilder::MoveRight);
	player_input->BindAxis("MoveUp", this, &ABuilder::MoveUp);
	player_input->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	player_input->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void ABuilder::MoveForward(float value)
{
	if (value != 0.f)
	{
		AddMovementInput(myCamera->GetForwardVector(), value);
	}
}

void ABuilder::MoveRight(float value)
{
	if (value != 0.f)
	{
		AddMovementInput(myCamera->GetRightVector(), value);
	}
}

void ABuilder::MoveUp(float value)
{
	if (value != 0.f)
	{
		AddMovementInput(FVector::UpVector, value);
	}
}

void ABuilder::OnFire_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("Fire"));
	
	if (myMesh)
	{
		FHitResult hit_result;
		FVector location = GetPlacementLocation(hit_result);

		if (!GetWorld()->LineTraceSingleByChannel(hit_result, hit_result.TraceStart, location, ECC_WorldStatic))
		{
			myISMC->AddInstanceWorldSpace(FTransform(FQuat::Identity, location));
		}
	}
}

FVector ABuilder::GetPlacementLocation (FHitResult& hit_result)
{
	FVector forward = Controller->GetActorForwardVector();
	FVector start = GetActorLocation();
	FVector size = myMesh->GetBounds().GetBox().GetSize();
	FVector location = start + forward * myRange;

	if (GetWorld()->LineTraceSingleByChannel(hit_result, start, location, ECC_WorldStatic))
	{
		location = hit_result.Location + hit_result.Normal * size;
	}

	location = Align(location, size);
	return location;
}

FVector ABuilder::Align (FVector location, const FVector& size)
{
	location = FVector(FIntVector(location / size)) * size;

	return location;
}
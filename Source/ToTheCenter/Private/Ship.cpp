// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"

// Sets default values
AShip::AShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	scene->SetupAttachment(GetRootComponent());

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(scene);
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShip::MoveShip(float joystickValue)
{
	AddActorLocalOffset(FVector(moveSpeed * joystickValue, 0.0, 0.0));
}

void AShip::TurnShip(float joystickValue)
{
	AddActorLocalRotation(FRotator(0.0, turnSpeed * joystickValue, 0.0));
	camera->AddRelativeRotation(FRotator(0.0, turnSpeed * joystickValue * -1, 0.0));
}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShip::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


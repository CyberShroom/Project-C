// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"

// Sets default values
AShip::AShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(scene);

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(scene);
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();
	
	shipInventory = NewObject<UInventory>();
	shipInventory->Initialize(8,currentHotbarSize * 2, EInventoryID::Hotbar_Inventory);
}

void AShip::MoveShip(float joystickValue)
{
	AddActorLocalOffset(FVector(moveSpeed * 200 * joystickValue, 0.0, 0.0));
}

void AShip::TurnShip(float joystickValue)
{
	AddActorLocalRotation(FRotator(0.0, turnSpeed * 75 * joystickValue, 0.0));
	camera->AddRelativeRotation(FRotator(0.0, turnSpeed * 75 * joystickValue * -1, 0.0));
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

void AShip::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShip, moveSpeed);
	DOREPLIFETIME(AShip, turnSpeed);
}


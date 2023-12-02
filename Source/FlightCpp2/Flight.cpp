// Fill out your copyright notice in the Description page of Project Settings.


#include "Flight.h"
#include "Flight.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AFlight::AFlight()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Flight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Flight"));
	SetRootComponent(Flight);
	Flight->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	SpringArmComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 5.0f));
	CameraComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

}

// Called when the game starts or when spawned
void AFlight::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetPlayerController(Flight, 0)->SetMouseLocation(0.0f, 0.0f);
	
}

// Called every frame
void AFlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UGameplayStatics::GetPlayerController(Flight, 0)->GetMousePosition(mouseX, mouseY);
	UE_LOG(LogTemp, Warning, TEXT("Mouse Location: %f, %f"), mouseX, mouseY);

}

void AFlight::Roll(float value)
{
	FVector torque = FVector(value * 999999, 0.f, 0.f);
	Flight->AddTorqueInRadians(GetActorQuat().RotateVector(torque));
}


void AFlight::Yaw(float value)
{
	FVector torque = FVector(0.f, value * 999999, 0.f);
	Flight->AddTorqueInRadians(GetActorQuat().RotateVector(torque));

}

void AFlight::Pitch(float value)
{
	FVector torque = FVector(0.f, 0.f, value * 999999);
	Flight->AddTorqueInRadians(GetActorQuat().RotateVector(torque));

}

// Called to bind functionality to input

void AFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Roll", this, &AFlight::Roll);
	PlayerInputComponent->BindAxis("Pitch", this, &AFlight::Pitch);
	PlayerInputComponent->BindAxis("Yaw", this, &AFlight::Yaw);
}



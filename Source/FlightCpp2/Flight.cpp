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

	viewSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	viewportSizeCenter = FVector2D(viewSize.X / 2, viewSize.Y / 2);
	pitchSpeed = 0.01;
	yawSpeed = 0.01;
	
}

// Called every frame
void AFlight::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	UGameplayStatics::GetPlayerController(Flight, 0)->GetMousePosition(mouseX, mouseY);
	UE_LOG(LogTemp, Warning, TEXT("%f"), roll);
	UE_LOG(LogTemp, Warning, TEXT("%s"), isSleep ? TEXT("true") : TEXT("false"));

	yaw = yawSpeed*((viewSize.X / 2) - mouseX);
	pitch = pitchSpeed*((viewSize.Y / 2) - mouseY);
	if (pitch > 2) {
		pitch = 2;
	}
	if (pitch < -2) {
		pitch = -2;
	}
	if (yaw > 2) {
		yaw = 2;
	}
	if (yaw < -2) {
		yaw = -2;
	}




	AFlight::Yaw(yaw);
	AFlight::Pitch(pitch);
	AFlight::Lift(AFlight::RayCast());
	AFlight::GWarning();
	AFlight::GetXRotation();
	AFlight::GetYRotation();
	AFlight::GetZRotation();
	if (isSleep) {
		AFlight::Sleep();
	}
	if (!isBoost) {
		if (engineSpeed > 90000) {
			engineSpeed -= 500;
		}
	}
}

void AFlight::GetXRotation(){
	FRotator rotation = GetActorRotation();
	roll = rotation.Roll;
}

void AFlight::GetYRotation() {
	FRotator rotation = GetActorRotation();
	if (rotation.Yaw > 0) {
		yawRotation = rotation.Yaw/360;
	}
	else {
		yawRotation = (rotation.Yaw /-360) + 0.5;
	}

}

void AFlight::GetZRotation() {
	FRotator rotation = GetActorRotation();
	pitchRotation = rotation.Pitch*pitchSpeed;
}

bool AFlight::RayCast() {
	FVector start = GetActorLocation();
	FVector end = start + (GetActorUpVector() * -100);
	FHitResult hit;
	if (GetWorld()) {
		bool hitBool = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Pawn);
		//DrawDebugLine(GetWorld(), start, end, FColor::Red, false,2.f, 0.f, 10.f);
		if (hitBool) {
			return true;
		}
		else {
			return false;
		}
	}
	return false;
}

void AFlight::GWarning() {
	if (yaw == 2 && engineSpeed > 90000|| pitch == 2 && engineSpeed > 90000 || yaw == -2 && engineSpeed > 90000 || pitch == -2 && engineSpeed > 90000) {
		gCounter -= GetWorld()->GetDeltaSeconds();
		warningTxt = "Warning High G";
		if (gCounter < 0) {
			isSleep = true;
		}
	}
}

void AFlight::Sleep() {
	if (isSleep) {
		sleepCounter -= GetWorld()->GetDeltaSeconds();
		pitch = 0;
		yaw = 0;
		engineSpeed -= 100;
		warningTxt = "sleeping: %i", sleepCounter;
		if (sleepCounter < 0) {
			isSleep = false;
			sleepCounter = 10.f;
			gCounter = 10.f;
			warningTxt = "";
		}
	}
}

void AFlight::Boosta(float value) {
	if (engineSpeed < 100000) {
		engineSpeed += value;
	}

	if (value > 0) {
		isBoost = true;
	}
	else {
		isBoost = false;
	}
	
	FVector force = FVector(engineSpeed, 0.f, 0.f);
	Flight->AddForce(GetActorQuat().RotateVector(force));
}

void AFlight::EngineSpeedUp(float value)
{
	if(engineSpeed < 90000){
		//UE_LOG(LogTemp, Warning, TEXT("%f"), engineSpeed);
		engineSpeed += value;
	}
	FVector force = FVector(engineSpeed, 0.f, 0.f);
	Flight->AddForce(GetActorQuat().RotateVector(force));

}
void AFlight::EngineSpeedDown(float value) {
	if (engineSpeed > 0) {
		engineSpeed -= value;
	}
	FVector force = FVector(engineSpeed, 0.f, 0.f);
	Flight->AddForce(GetActorQuat().RotateVector(force));
}

void AFlight::Roll(float value)
{
	FVector torque = FVector(value * 999999, 0.f, 0.f);
	Flight->AddTorqueInRadians(GetActorQuat().RotateVector(torque));
}


void AFlight::Pitch(float value)
{
	FVector torque = FVector(0.f, value * 90000, 0.f);
	Flight->AddTorqueInRadians(GetActorQuat().RotateVector(torque));

}

void AFlight::Yaw(float value)
{
	FVector torque = FVector(0.f, 0.f, value * 90000);
	Flight->AddTorqueInRadians(-GetActorQuat().RotateVector(torque));

}

void AFlight::Lift(bool value) {
	float mass = Flight->GetMass();
	if (engineSpeed > 50000 && value) {
		FVector force = FVector(0.f, 0.f, mass * 980 + 10000);
		Flight->AddForce(force);
	}
	if (engineSpeed > 50000) {
		FVector force = FVector(0.f, 0.f, mass * 980);
		Flight->AddForce(force);
	}

	
}

// Called to bind functionality to input

void AFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Roll", this, &AFlight::Roll);
	PlayerInputComponent->BindAxis("SpeedIntervalUp", this, &AFlight::EngineSpeedUp);
	PlayerInputComponent->BindAxis("SpeedIntervalDown", this, &AFlight::EngineSpeedDown);
	PlayerInputComponent->BindAxis("Boost", this, &AFlight::Boosta);
	//PlayerInputComponent->BindAxis("Yaw", this, &AFlight::Yaw;
}



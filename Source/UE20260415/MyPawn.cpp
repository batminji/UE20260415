// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyRocket.h"
#include "MyRotateStaticMeshComponent.h"
#include "EnhancedInputComponent.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(45.f, 60.f, 20.f));

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(TEXT("/Script/Engine.StaticMesh'/Game/Assets/P38/SM_P38_Body.SM_P38_Body'"));
	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}

	Left = CreateDefaultSubobject<UMyRotateStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Body);
	Left->SetRelativeLocation(FVector(37.f, -21.f, 0.f));
	Left->RotationSpeed = 720.0f;

	Right = CreateDefaultSubobject<UMyRotateStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Body);
	Right->SetRelativeLocation(FVector(37.f, 21.f, 0.f));
	Right->RotationSpeed = 720.0f;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Propeller(TEXT("/Script/Engine.StaticMesh'/Game/Assets/P38/SM_P38_Propeller.SM_P38_Propeller'"));
	if (SM_Propeller.Succeeded())
	{
		Left->SetStaticMesh(SM_Propeller.Object);
		Right->SetStaticMesh(SM_Propeller.Object);
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Box);
	SpringArm->SocketOffset = FVector(0.f, 0.f, 33.33f);
	SpringArm->TargetArmLength = 120.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Box);
	Arrow->SetRelativeLocation(FVector(200.f, 0.f, 0.f));

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	FloatingPawnMovement->MaxSpeed = MoveSpeed;

	static ConstructorHelpers::FClassFinder<AMyRocket> BP_Rocket(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Rocket.BP_Rocket_C'"));
	if (BP_Rocket.Succeeded())
	{
		RocketClass = BP_Rocket.Class;
	}
}

void AMyPawn::Rotate(const FInputActionValue& Value)
{
	FVector2D RotateValue = Value.Get<FVector2D>() * RotationSpeed * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	AddActorLocalRotation(FRotator(RotateValue.Y, 0.0f, RotateValue.X));
}

void AMyPawn::Fire()
{
	GetWorld()->SpawnActor<AMyRocket>(RocketClass, Arrow->K2_GetComponentToWorld());
}

void AMyPawn::Boost()
{
	BoostValue = 1.0f;
}

void AMyPawn::UnBoost()
{
	BoostValue = 0.5f;
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(GetActorForwardVector(), BoostValue);
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(EIC)
	{
		EIC->BindAction(IA_Rotate, ETriggerEvent::Triggered, this, &AMyPawn::Rotate);
		EIC->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &AMyPawn::Fire);
		EIC->BindAction(IA_Boost, ETriggerEvent::Triggered, this, &AMyPawn::Boost);
		EIC->BindAction(IA_Boost, ETriggerEvent::Canceled, this, &AMyPawn::UnBoost);
	}
}


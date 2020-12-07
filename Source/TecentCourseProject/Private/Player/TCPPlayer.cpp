// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/TCPPlayer.h"

// Sets default values
ATCPPlayer::ATCPPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	//GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpingArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	CamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
	CamComp->SetupAttachment(SpringArmComp);
	ZoomFOV = 65.0f;
	ZoomSpeeed = 2.0f;
}

// Called when the game starts or when spawned
void ATCPPlayer::BeginPlay()
{
	Super::BeginPlay();
	DefaultFOV = CamComp->FieldOfView;
}

// Called every frame
void ATCPPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const float CurrentFOV = bWantToZoom ? ZoomFOV:DefaultFOV;
	float thisFOV = FMath::FInterpTo(CamComp->FieldOfView,CurrentFOV,DeltaTime,ZoomSpeeed);
	CamComp->SetFieldOfView(thisFOV);
}

// Called to bind functionality to input
void ATCPPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward",this,&ATCPPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&ATCPPlayer::MoveRight);
	PlayerInputComponent->BindAxis("LookUp",this,&ATCPPlayer::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn",this,&ATCPPlayer::AddControllerYawInput);
	
	PlayerInputComponent->BindAction("Crouch",IE_Pressed,this,&ATCPPlayer::BeginCrouch);
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ATCPPlayer::Jump);
	PlayerInputComponent->BindAction("Crouch",IE_Released,this,&ATCPPlayer::EndCrouch);
	PlayerInputComponent->BindAction("Zoom",IE_Pressed,this,&ATCPPlayer::BeginZoom);
	PlayerInputComponent->BindAction("Zoom",IE_Released,this,&ATCPPlayer::EndZoom);
	
}

FVector ATCPPlayer::GetPawnViewLocation() const
{
	if(CamComp)
	{
		return CamComp->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

void ATCPPlayer::MoveForward(float val)
{
	AddMovementInput(GetActorForwardVector() * val);
}

void ATCPPlayer::MoveRight(float val)
{
	AddMovementInput(GetActorRightVector() * val);
}

void ATCPPlayer::BeginCrouch()
{
	Crouch();
}

void ATCPPlayer::EndCrouch()
{
	UnCrouch();
}

void ATCPPlayer::BeginZoom()
{
	bWantToZoom = true;
}

void ATCPPlayer::EndZoom()
{
	bWantToZoom = false;
}


// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/TCPPlayer.h"
#include"Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
// Sets default values
ATCPPlayer::ATCPPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	//GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpingArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	
	/*
	 * 设置摄像头
	 */
	CamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
	CamComp->SetupAttachment(SpringArmComp);
	/*
	 * 聚焦相关变量初始化
	 */
	ZoomFOV = 65.0f;
	ZoomSpeeed = 2.0f;

	SocketName = "WeaponSocket";
}

// Called when the game starts or when spawned
void ATCPPlayer::BeginPlay()
{
	Super::BeginPlay();
	DefaultFOV = CamComp->FieldOfView;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurWeapon = GetWorld()->SpawnActor<ATCPWeapon>(InitWeaponClass,FVector::ZeroVector,FRotator::ZeroRotator,SpawnParameters);
	if(CurWeapon)
	{
		CurWeapon->SetOwner(this);
		CurWeapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,SocketName);
	}
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
	PlayerInputComponent->BindAction("Fire",IE_Pressed,this,&ATCPPlayer::Fire);
}
/*
 * 重写获取摄像机镜头函数
 */
FVector ATCPPlayer::GetPawnViewLocation() const
{
	if(CamComp)
	{
		return CamComp->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}
/*
 * 前后移动
 */
void ATCPPlayer::MoveForward(float val)
{
	AddMovementInput(GetActorForwardVector() * val);
}
/*
 * 左右移动
 */
void ATCPPlayer::MoveRight(float val)
{
	AddMovementInput(GetActorRightVector() * val);
}
/*
 * 开始下蹲
 */
void ATCPPlayer::BeginCrouch()
{
	Crouch();
}
/*
 * 结束下蹲
 */
void ATCPPlayer::EndCrouch()
{
	UnCrouch();
}
/*
 * 开始聚焦
 */
void ATCPPlayer::BeginZoom()
{
	bWantToZoom = true;
}
/*
 * 结束聚焦
 */
void ATCPPlayer::EndZoom()
{
	bWantToZoom = false;
}

void ATCPPlayer::Fire()
{
		if(CurWeapon)
		{
			CurWeapon->Fire();
		}
}


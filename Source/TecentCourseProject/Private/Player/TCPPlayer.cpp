// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/TCPPlayer.h"
#include"Components/SkeletalMeshComponent.h"
#include "Player/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
// Sets default values
ATCPPlayer::ATCPPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	//GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpingArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	HPComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HelathComp"));
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
	bDied = false;
	SocketName = "WeaponSocket";
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	bWantToCourch =false;
}

//Called when the game starts or when spawned
void ATCPPlayer::BeginPlay()
{
	Super::BeginPlay();
	DefaultFOV = CamComp->FieldOfView;
	HPComp->OnHealthChanged.AddDynamic(this,&ATCPPlayer::OnHealthChanged);
	if(GetLocalRole() == ROLE_Authority)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		CurWeapon = GetWorld()->SpawnActor<ATCPWeapon>(InitWeaponClass,FVector::ZeroVector,FRotator::ZeroRotator,SpawnParameters);
		if(CurWeapon)
		{
			CurWeapon->SetOwner(this);
			CurWeapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,SocketName);
		}
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
	PlayerInputComponent->BindAxis("TurnRate", this, &ATCPPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATCPPlayer::LookUpAtRate);
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

void ATCPPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATCPPlayer,CurWeapon);
	DOREPLIFETIME(ATCPPlayer,bDied);
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

void ATCPPlayer::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


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

void ATCPPlayer::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
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

bool ATCPPlayer::GetIsAiming()
{
	return bWantToZoom;
}

ATCPWeapon* ATCPPlayer::GetWeapon() const
{
	return CurWeapon;
}


void ATCPPlayer::OnHealthChanged(class UHealthComponent* HealthComp,float HP,float HPDelta,const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{

	if(HP <=0.0f && !bDied)
	{
		bDied = true;
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.0f);
		
	}
	
}



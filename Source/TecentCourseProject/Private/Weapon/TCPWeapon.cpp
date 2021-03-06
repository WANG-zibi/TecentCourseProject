// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/TCPWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Player/TCPPlayer.h"
// Sets default values

ATCPWeapon::ATCPWeapon()
{

	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	BaseDamage = 20.0f;
	RootComponent = WeaponMesh;
	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";
	BulletSpread = 2.0f;
	SetReplicates(true);
	NetUpdateFrequency = 66;
	MinNetUpdateFrequency = 33;
	RateOfFire = 100.0f;
	TimeBetweenShots = 60/RateOfFire;
}
void ATCPWeapon::BeginPlay()
{
	Super::BeginPlay();
}
void ATCPWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimeHandle_TimeShots);
}

void ATCPWeapon::StartFire()
{
	Fire();
	// float FirstDelay = LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds;
	// FirstDelay = FMath::Max(FirstDelay,0.0f);
	// 	GetWorldTimerManager().ClearTimer(TimeHandle_TimeShots);
	// GetWorldTimerManager().SetTimer(TimeHandle_TimeShots,this,&ATCPWeapon::Fire,TimeBetweenShots,true,FirstDelay);
}

// Called when the game starts or when spawned


void ATCPWeapon::Fire()
{
	if(GetLocalRole() < ROLE_Authority)
	{
		ServerFire();
	}

		AActor* CurOwner = GetOwner();
		if(CurOwner)
		{
			FVector EyeLocation;
			FRotator EyeRotation;
			CurOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);
		
			FVector ShotDirection = EyeRotation.Vector();
			
			// Bullet Spread
			float HalfRad = FMath::DegreesToRadians(BulletSpread);
			ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);
			FVector TraceEnd = EyeLocation + (ShotDirection * 10000);
			FHitResult Hit;
		
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(CurOwner);
			QueryParams.AddIgnoredActor(this);
			QueryParams.bTraceComplex = true;
			QueryParams.bReturnPhysicalMaterial=true;
			FVector TracerEndPoint = TraceEnd;
			auto PhysicSurface = SurfaceType_Default;
			if(GetWorld()->LineTraceSingleByChannel(Hit,EyeLocation,TraceEnd,ECC_Visibility,QueryParams))
			{
				AActor* HitActor = Hit.GetActor();
				
				UGameplayStatics::ApplyPointDamage(HitActor,BaseDamage,ShotDirection,Hit,CurOwner->GetInstigatorController(),this,DamageTye);
				PhysicSurface = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
				
				HitEffect(Hit.ImpactPoint,PhysicSurface);
				TracerEndPoint = Hit.ImpactPoint;
				
				
			}
		
			//DrawDebugLine(GetWorld(),EyeLocation,TraceEnd,FColor::White,false,2.0f,0,1.0f);
			WeaponEffect(TracerEndPoint);

			if(GetLocalRole() == ROLE_Authority)
			{
				HitScanTrace.TraceTo = TracerEndPoint;
				HitScanTrace.SurfaceType = PhysicSurface;
			}
			LastFireTime = GetWorld()->TimeSeconds;
		}
}

void ATCPWeapon::ServerFire_Implementation()
{
	Fire();
}

bool ATCPWeapon::ServerFire_Validate()
{
	return true;
}

void ATCPWeapon::OnRep_HitScanTrace()
{
	WeaponEffect(HitScanTrace.TraceTo);
	HitEffect(HitScanTrace.TraceTo,HitScanTrace.SurfaceType);
}

// Called every frame
void ATCPWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATCPWeapon::WeaponEffect(FVector& TracerEndPoint)
{
	if(MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect,WeaponMesh,MuzzleSocketName);
	}
	
	FVector MuzzleLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);
	if(TracerEffect)
	{
		UParticleSystemComponent* TracerComp =UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),TracerEffect,MuzzleLocation);
		if(TracerComp)
		{
			TracerComp->SetVectorParameter(TracerTargetName,TracerEndPoint);
		}
	}
	APawn* MyActor = Cast<APawn>(GetOwner());
	if(MyActor)
	{
		auto PC =Cast<APlayerController>(MyActor->GetController());
		if(PC)
		{
			PC->ClientPlayCameraShake(OpenFireShake);
		}
	}
}

void ATCPWeapon::HitEffect(FVector ImpactPoint,EPhysicalSurface PhysicSurface)
{
	UParticleSystem* SelectEffect = nullptr;
	switch (PhysicSurface)
	{
	case SurfaceType1:
		SelectEffect = FleshImpactEffect;
		break;
	case SurfaceType2:
		SelectEffect = FleshImpactEffect;
		break;
	case SurfaceType3:
		break;
	default:
		SelectEffect = DefaultImpactEffect;
		break;
	}
	if(SelectEffect)
	{
		FVector MuzzleLoca = WeaponMesh->GetSocketLocation(MuzzleSocketName);
		FVector ShotDir = ImpactPoint - MuzzleLoca;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),SelectEffect,ImpactPoint,ShotDir.Rotation());
	}
	
}



void ATCPWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ATCPWeapon,HitScanTrace,COND_SkipOwner);
}


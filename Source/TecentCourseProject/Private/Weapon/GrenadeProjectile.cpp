// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GrenadeProjectile.h"
#include "Components\SphereComponent.h"
#include "GameFrameWork\ProjectileMovementComponent.h"
#include "PhysicsEngine\RadialForceComponent.h"
#include "Particles\ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Net/UnrealNetwork.h"
void AGrenadeProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		MeshComp->OnComponentHit.AddDynamic(this, &AGrenadeProjectile::OnHit);
	}
	Shoot();
}

// Sets default values
AGrenadeProjectile::AGrenadeProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	InitImpulse = 3000.f;
	ImpulseStrength = 3000.f;
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetAutoActivate(false);
	RadialForceComp->ImpulseStrength = ImpulseStrength;
	
	RadialForceComp->SetupAttachment(RootComponent);
	SpeedThreshold = 3000.f;
	DamageType = UDamageType::StaticClass();
	Damage = 100.f;
	DamageRadius = 1000.f;
	

	SetReplicates(true);
}

void AGrenadeProjectile::Shoot()
{
	const FRotator ProjectileRotator = GetActorRotation();
	MeshComp->AddImpulse(GetActorRotation().Vector()*InitImpulse, NAME_None, true);
}

void AGrenadeProjectile::Explode(FVector ExplodeLocation)
{
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	UGameplayStatics::ApplyRadialDamage(this, Damage, ExplodeLocation, DamageRadius, DamageType, IgnoreActors, this, GetOwner()->GetInstigatorController(), true,ECC_Visibility);
	UE_LOG(LogTemp, Warning, TEXT("biaobiaobiaobiaobiaobiao"));
	this->Destroy();
}

bool AGrenadeProjectile::IsToExplode() const
{
	const float CurrentVelocity = GetVelocity().Size();
	if (CurrentVelocity <= 0.1f) 
		return false;
	return CurrentVelocity <= SpeedThreshold;
}

void AGrenadeProjectile::SlowDown(EPhysicalSurface HitSurfaceType)
{
	const float DecayFactor = 1.0f;
	switch (HitSurfaceType)
	{
	case SurfaceType_Default:
    case SurfaceType1:
    case SurfaceType2:
        break;
	default:
		break;
	}
	FVector CurrentVelocity = GetVelocity();
	CurrentVelocity *= DecayFactor;
	MeshComp->SetAllPhysicsLinearVelocity(CurrentVelocity);
}


void AGrenadeProjectile::Destroyed()
{
	FVector ExplodeLocation = GetActorLocation();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, ExplodeLocation);
	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplodeSound, ExplodeLocation);
	RadialForceComp->FireImpulse();
}

// Called every frame
void AGrenadeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority())
		{
		FVector CurrentV = GetVelocity();
		if (IsToExplode()) {
			Explode(MeshComp->GetComponentLocation());
		}
	}
}

void AGrenadeProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	 EPhysicalSurface HitSurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
	 if (IsHitTarget(HitSurfaceType))
	 	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, DamageType);
		Explode(Hit.ImpactPoint);
	}
	else
		{
		SlowDown(HitSurfaceType);
		this->Destroy();
	}
	
}

void AGrenadeProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME_CONDITION(AGrenadeProjectile,OnHit,COND_SkipOwner);
}


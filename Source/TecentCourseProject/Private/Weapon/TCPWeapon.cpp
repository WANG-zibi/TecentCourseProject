// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/TCPWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
// Sets default values

ATCPWeapon::ATCPWeapon()
{

	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";
}

// Called when the game starts or when spawned
void ATCPWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATCPWeapon::Fire()
{
	AActor* CurOwner = GetOwner();
	if(CurOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		CurOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);
		
		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (ShotDirection * 10000);
		
		FHitResult Hit;
		
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(CurOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		FVector TracerEndPoint = TraceEnd;
		if(GetWorld()->LineTraceSingleByChannel(Hit,EyeLocation,TraceEnd,ECC_Visibility,QueryParams))
		{
			AActor* HitActor = Hit.GetActor();
			UGameplayStatics::ApplyPointDamage(HitActor,20.0f,ShotDirection,Hit,CurOwner->GetInstigatorController(),this,DamageTye);
			if(ImpactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactEffect,Hit.ImpactPoint,Hit.ImpactNormal.Rotation());
			}
			TracerEndPoint = Hit.ImpactPoint;
		}
		
		//DrawDebugLine(GetWorld(),EyeLocation,TraceEnd,FColor::White,false,2.0f,0,1.0f);
		WeaponEffect(TracerEndPoint);
	}
	
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


// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/TCPGrenadeWeapon.h"

void ATCPGrenadeWeapon::Fire()
{
	AActor* CurOwner = GetOwner();
	if(CurOwner && ProjectileClass)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		CurOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);
		const FVector MuzzleLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);
		FRotator MuzzleRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation,EyeRotation,SpawnParameters);
	}
}

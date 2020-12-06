// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TCPWeapon.generated.h"
class USkeletalMeshComponent;
class UParticleSystem;
UCLASS()
class TECENTCOURSEPROJECT_API ATCPWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATCPWeapon();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable,Category="Weapon")
	virtual void Fire();


	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon")
	TSubclassOf<class UDamageType>DamageTye;
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Weapon")
	FName MuzzleSocketName;
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Weapon")
	FName TracerTargetName;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon")
	UParticleSystem* MuzzleEffect;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon")
	UParticleSystem* ImpactEffect;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon")
	UParticleSystem* TracerEffect;
UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Components")
	USkeletalMeshComponent* WeaponMesh;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

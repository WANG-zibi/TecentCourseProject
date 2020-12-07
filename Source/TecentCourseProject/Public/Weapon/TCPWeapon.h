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
	UFUNCTION(BlueprintCallable,Category="Weapon")
    virtual void Fire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon")
	TSubclassOf<class UDamageType>DamageTye;
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Weapon")
	FName MuzzleSocketName;
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Weapon")
	FName TracerTargetName;
	/*
	 * 特效相关变量、指针
	 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Effect")
	UParticleSystem* MuzzleEffect;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Effect")
	UParticleSystem* DefaultImpactEffect;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Effect")
	UParticleSystem* FleshImpactEffect;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Effect")
	UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly,Category="Weapon")
	TSubclassOf<UCameraShake> OpenFireShake;
	/*
	 * 武器Mesh
	 */
UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Components")
	USkeletalMeshComponent* WeaponMesh;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void WeaponEffect(FVector& TracerEndPoint);
	
	
};

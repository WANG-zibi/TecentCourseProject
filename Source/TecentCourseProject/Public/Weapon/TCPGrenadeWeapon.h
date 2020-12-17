// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/TCPWeapon.h"
#include "TCPGrenadeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TECENTCOURSEPROJECT_API ATCPGrenadeWeapon : public ATCPWeapon
{
	GENERATED_BODY()
protected:
	virtual void Fire() override;
	UPROPERTY(EditDefaultsOnly,Category="zidan")
	TSubclassOf<AActor> ProjectileClass;

	UFUNCTION(Server,Reliable,WithValidation)
    void ServerShoot();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

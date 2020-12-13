// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/TCPWeapon.h"
#include "TCPPlayer.generated.h"

UCLASS()
class TECENTCOURSEPROJECT_API ATCPPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATCPPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/*
	 * 移动控制部分
	 */
	void MoveForward(float val);
	void MoveRight(float val);
	void LookUpAtRate(float Rate);
	UFUNCTION()
	void BeginCrouch();
	UFUNCTION()
	void EndCrouch();

	void BeginZoom();
	void TurnAtRate(float Rate);
	void EndZoom();

	UPROPERTY(Replicated,VisibleDefaultsOnly,Category="Weapon")
	ATCPWeapon* CurWeapon;
	UPROPERTY(EditDefaultsOnly,Category="Weapon")
	TSubclassOf<ATCPWeapon>InitWeaponClass;
	UPROPERTY(VisibleDefaultsOnly,Category="Weapon")
	FName SocketName;
	
	void Fire();

    UFUNCTION()
	bool GetIsAiming();
	
	UFUNCTION(BlueprintCallable)
	ATCPWeapon* GetWeapon() const;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UCameraComponent* CamComp;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	class UHealthComponent* HPComp;

	UFUNCTION()
	void OnHealthChanged(class UHealthComponent* HealthComp,float HP,float HPDelta,const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	UPROPERTY(Replicated,BlueprintReadOnly,Category="Player")
	bool bDied;
	UPROPERTY(BlueprintReadWrite)
	bool bWantToZoom;

	UPROPERTY(BlueprintReadWrite)
	bool bWantToCourch;
	UPROPERTY(EditDefaultsOnly,Category="Zoom",meta = (ClampMin = 0.1,ClampMax = 100))
	//聚焦速度
	float ZoomSpeeed;
	UPROPERTY(EditDefaultsOnly,Category="Zoom")
	//聚焦深度
	float ZoomFOV;
	//默认镜头深度
	float DefaultFOV;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FVector GetPawnViewLocation() const override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

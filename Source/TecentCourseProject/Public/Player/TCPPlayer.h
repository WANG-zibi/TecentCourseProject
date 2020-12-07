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
	void BeginCrouch();
	void EndCrouch();

	void BeginZoom();
	void EndZoom();


	UPROPERTY(VisibleDefaultsOnly,Category="Weapon")
	ATCPWeapon* CurWeapon;
	UPROPERTY(EditDefaultsOnly,Category="Weapon")
	TSubclassOf<ATCPWeapon>InitWeaponClass;
	UPROPERTY(VisibleDefaultsOnly,Category="Weapon")
	FName SocketName;
	
	void Fire();
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UCameraComponent* CamComp;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USpringArmComponent* SpringArmComp;

	
	bool bWantToZoom;
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
	
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
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


	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UCameraComponent* CamComp;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USpringArmComponent* SpringArmComp;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FVector GetPawnViewLocation() const override;
};

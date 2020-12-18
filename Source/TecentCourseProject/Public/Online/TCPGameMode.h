// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TCPGameMode.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController);
enum class EWaveState : uint8;
UCLASS()
class TECENTCOURSEPROJECT_API ATCPGameMode : public AGameMode
{
	GENERATED_BODY()
	
protected:

	FTimerHandle TimerHandle_ReBorn;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float TimeBetweenWaves;
	

	void RestartDeadPlayers();

public:
	ATCPGameMode();
	virtual void StartPlay() override;
	
	// virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintAssignable, Category = "GameMode")
	FOnActorKilled OnActorKilled;
	
};

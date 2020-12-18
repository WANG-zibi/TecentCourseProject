// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TCPGameState.generated.h"
UENUM(BlueprintType)
enum class EWaveState : uint8
{
	WaitingToStart,

    WaveInProgress,

    // No longer spawning new bots, waiting for players to kill remaining bots
    WaitingToComplete,

    WaveComplete,

    GameOver,
};
/**
 * 
 */
UCLASS()
class TECENTCOURSEPROJECT_API ATCPGameState : public AGameState
{
	GENERATED_BODY()
	protected:
	
	UFUNCTION()
    void OnRep_WaveState(EWaveState OldState);
	UFUNCTION(BlueprintImplementableEvent, Category = "GameState")
    void WaveStateChanged(EWaveState NewState, EWaveState OldState);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WaveState, Category = "GameState")
	EWaveState WaveState;
	public:
	void SetWaveState(EWaveState NewState);
};

// Fill out your copyright notice in the Description page of Project Settings.

#include "Online/TCPGameMode.h"
#include "Player/tcpPlayerState.h"
#include "TimerManager.h"
#include "Online/TCPGameState.h"


ATCPGameMode::ATCPGameMode()
{
	TimeBetweenWaves = 5.0f;
	
	GameStateClass = ATCPGameMode::StaticClass();
	PlayerStateClass = ATCPGameMode::StaticClass();
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

void ATCPGameMode::StartPlay()
{
	Super::StartPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_ReBorn, this, &ATCPGameMode::RestartDeadPlayers, TimeBetweenWaves, true);
}



void ATCPGameMode::RestartDeadPlayers()
{

	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		auto PC = It->Get();
		if (PC && PC->GetPawn() == nullptr)
		{
			RestartPlayer(PC);
		}
	}
	
}


void ATCPGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
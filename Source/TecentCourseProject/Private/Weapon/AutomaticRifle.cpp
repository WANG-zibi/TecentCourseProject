// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/AutomaticRifle.h"
#include "Runtime/Engine/Public/TimerManager.h"

AAutomaticRifle::AAutomaticRifle()
{
	RateOfFire = 600.0f;
	TimeBetweenShots = 60/RateOfFire;
}

void AAutomaticRifle::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimeHandle_TimeShots);
}

void AAutomaticRifle::StartFire()
{
	float FirstDelay = LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds;
	FirstDelay = FMath::Max(FirstDelay,0.0f);
	GetWorldTimerManager().SetTimer(TimeHandle_TimeShots,this,&AAutomaticRifle::Fire,TimeBetweenShots,true,FirstDelay);
}

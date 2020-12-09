// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HealthComponent.h"

#include "GameFramework/GameModeBase.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	DefaultHP = 100;
	HP = DefaultHP;
	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* MyOwner = GetOwner();
	if(MyOwner)
	{

		MyOwner->OnTakeAnyDamage.AddDynamic(this,&UHealthComponent::TakeDamage);
	}
	// ...
	
}


void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if(Damage <= 0.0f) return;

	HP = FMath::Clamp(HP - Damage,0.0f,DefaultHP);
	UE_LOG(LogTemp,Warning,TEXT("Health Changed:%s"),*FString::SanitizeFloat(HP));
	OnHealthChanged.Broadcast(this,HP,Damage,DamageType,InstigatedBy,DamageCauser);
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

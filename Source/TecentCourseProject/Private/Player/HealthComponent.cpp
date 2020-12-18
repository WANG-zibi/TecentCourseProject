// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HealthComponent.h"
#include "Net/UnrealNetwork.h"

#include "GeneratedCodeHelpers.h"
#include "GameFramework/GameModeBase.h"
#include "Online/TCPGameMode.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	DefaultHP = 100;
	HP = DefaultHP;
	bIsDead = false;

	this->SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	if(GetOwnerRole() == ROLE_Authority)
	{
		AActor* MyOwner = GetOwner();
		if(MyOwner)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this,&UHealthComponent::TakeDamage);
		}
	}
	// ...
	
}

void UHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f || HP <= 0.0f)
	{
		return;
	}

	HP = FMath::Clamp(HP + HealAmount, 0.0f, DefaultHP);

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s (+%s)"), *FString::SanitizeFloat(HP), *FString::SanitizeFloat(HealAmount));

	OnHealthChanged.Broadcast(this, HP, -HealAmount, nullptr, nullptr, nullptr);
}


void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if(Damage <= 0.0f) return;
	HP = FMath::Clamp(HP - Damage,0.0f,DefaultHP);
	UE_LOG(LogTemp,Warning,TEXT("Health Changed:%s"),*FString::SanitizeFloat(HP));
	OnHealthChanged.Broadcast(this,HP,Damage,DamageType,InstigatedBy,DamageCauser);
	bIsDead = HP <= 0.0f;
		if(bIsDead)
		{
			auto GM = Cast<ATCPGameMode>(GetWorld()->GetAuthGameMode());
			if (GM)
			{
				GM->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);
			}
		}
	
}

float UHealthComponent::GetHP() const
{
	return HP;
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent,HP);
}
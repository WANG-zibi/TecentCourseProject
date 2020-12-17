// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrenadeProjectile.generated.h"
class USphereComponent;
class UProjectileMovementComponent;
class URadialForceComponent;
class UParticleSystem;
class UDamageType;

UCLASS()
class TECENTCOURSEPROJECT_API AGrenadeProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	// Sets default values for this actor's properties
	AGrenadeProjectile();
	// 给抛射体一个初速度
	void Shoot();
	void Explode(FVector ExplodeLocation);
	
	bool IsToExplode() const;

	void SlowDown(EPhysicalSurface HitSurfaceType);
	FRotator initRotator;
	FORCEINLINE bool IsHitTarget(EPhysicalSurface HitSurfaceType) const
	{
		return HitSurfaceType == SurfaceType1 || HitSurfaceType == SurfaceType2;
	};

	virtual void Destroyed() override;
	public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	URadialForceComponent* RadialForceComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	USoundBase* ExplodeSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float InitImpulse;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float DamageRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UDamageType> DamageType;

	// 碰撞事件
	UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	float SpeedThreshold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	float ImpulseStrength;

	protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectile.h"
#include "SMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class USoundBase;

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASMagicProjectile();

protected:

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SFX")
	UAudioComponent* AudioComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SFX")
	USoundBase* ImpactSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Impact")
	TSubclassOf<UCameraShakeBase> ImpactShake;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float DeltaHealthAmount;

	void Explode(AActor* OtherActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

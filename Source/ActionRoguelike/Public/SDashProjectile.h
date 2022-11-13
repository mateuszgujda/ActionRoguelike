// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectile.h"
#include "SDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectile
{
	GENERATED_BODY()


	FTimerHandle SpawnLifetime_Handle;

protected:


	UPROPERTY(VisibleAnywhere)
	float ProjectileLifetime;


	UPROPERTY(VisibleAnywhere)
	float TeleportOffset;

	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	void TeleportInstigator();
	virtual void Explode_Implementation() override;

public:
	ASDashProjectile();

	virtual void BeginPlay() override;
};

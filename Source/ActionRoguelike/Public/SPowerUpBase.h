// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SPowerUpBase.generated.h"

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASPowerUpBase : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerUpBase();


	virtual void Interact_Implementation(APawn* InstigatorPawn);
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recharge")
	float RechargeTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Recharge")
	bool bIsRecharging;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boost")
	float BoostValue;

	FTimerHandle RechargeTimeHandle;
	ECollisionEnabled::Type DefaultCollision;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void StartRecharge();
	virtual void EndRecharge();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

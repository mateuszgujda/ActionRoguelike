// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUpBase.h"
#include "SAction.h"
#include "SAbilityPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASAbilityPowerUp : public ASPowerUpBase
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<USAction> ActionToGrant;

	void Interact_Implementation(APawn* InstigatorPawn) override;

	void PostInitializeComponents() override;
};

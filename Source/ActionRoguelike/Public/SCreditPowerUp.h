// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUpBase.h"
#include "SCreditPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASCreditPowerUp : public ASPowerUpBase
{
	GENERATED_BODY()
	
	void Interact_Implementation(APawn* InstigatorPawn) override;
protected:
	virtual void StartRecharge() override;
	virtual void EndRecharge() override;
	void PostInitializeComponents() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUpBase.h"
#include "SHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASPowerUpBase
{
	GENERATED_BODY()
	

protected:
	virtual void StartRecharge() override;
};

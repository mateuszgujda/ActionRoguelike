// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreditsChanged, float, NewCredits);
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
	

protected:
	float Credits;

public:
	ASPlayerState();
	void ApplyCreditsChange(float DeltaCredits);
	float GetCredits() const;

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;
};

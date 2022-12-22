// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ASPlayerState*, PlayerState, float, NewCredits, float, Delta);
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing="OnRep_Credits", Category = "Credits")
	float Credits;

	UFUNCTION()
	void OnRep_Credits(float OldCredits);

public:
	ASPlayerState();
	void ApplyCreditsChange(float DeltaCredits);
	UFUNCTION(BlueprintCallable)
	float GetCredits() const;

	

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;

};

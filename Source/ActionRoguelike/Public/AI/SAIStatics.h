// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SAIStatics.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAIStatics : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, category="AI Statics")
	static const FORCEINLINE FName GetAICharacterMuzzleName()
	{
		return AI_CHARACTER_MUZZLE_NAME;
	}

	UFUNCTION(BlueprintPure, category = "AI Statics")
		static const FORCEINLINE FName GetAITargetActorKeyName()
	{
		return AI_TARGET_ACTOR_KEY_NAME;
	}

	UFUNCTION(BlueprintPure, category = "AI Statics")
		static const FORCEINLINE FName GetAIMoveToLocationKeyName()
	{
		return AI_MOVE_TO_LOCATION_KEY_NAME;
	}

	UFUNCTION(BlueprintPure, category = "AI Statics")
		static const FORCEINLINE FName GetAIIsOnLowHealthKeyName()
	{
		return AI_IS_ON_LOW_HEALTH_KEY_NAME;
	}

private:
	static const FName AI_CHARACTER_MUZZLE_NAME;
	static const FName AI_TARGET_ACTOR_KEY_NAME;
	static const FName AI_MOVE_TO_LOCATION_KEY_NAME;
	static const FName AI_IS_ON_LOW_HEALTH_KEY_NAME;
};

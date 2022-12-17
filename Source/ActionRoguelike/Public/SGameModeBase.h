// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SGameModeBase.generated.h"


class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class ASPowerUpBase;

USTRUCT()
struct FPowerUpData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASPowerUpBase> powerUp;

	UPROPERTY(EditDefaultsOnly)
	float weight;

	FPowerUpData() {
		weight = 0.0f;
		powerUp = nullptr;

	}
};

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	
	FTimerHandle TimerHandle_SpawnBots;
	FTimerHandle TimerHandle_PowerUps;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "Buffs")
	UEnvQuery* SpawnBuffsQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Buffs")
	UCurveFloat* BuffAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Buffs")
	TArray<FPowerUpData> PowerUpsToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Buffs")
	float PowerUpsTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Respawn")
	float RespawnDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	float CreditPointsForKill;

	UFUNCTION()
	void OnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void RespawnPlayerElapsed(AController* PlayerController);

	UFUNCTION()
	int PickRandomPowerUpIndex();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Buffs")
	float SumOfBuffWeights;

	UFUNCTION()
	void SpawnPowerUpTimerElapsed();

	UFUNCTION()
	void OnPowerUpsQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

public:
	ASGameModeBase();

	virtual void StartPlay() override;

	virtual void OnActorKilled(AActor* VictimActor, AActor* KillerActor);

	UFUNCTION(Exec)
	void KillAllAI();
};

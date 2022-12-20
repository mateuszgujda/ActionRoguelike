// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "EngineUtils.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "SPowerUpBase.h"
#include <Kismet/GameplayStatics.h>


static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable Spawning of Bots via Timer"), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarSpawnPowerUps(TEXT("su.SpawnPowerUps"), true, TEXT("Enable Spawnig of PowerUps via Timer"), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	RespawnDelay = 2.0f;
	CreditPointsForKill = 20.0f;
	SumOfBuffWeights = 0.0f;
	PowerUpsTimerInterval = 2.0f;
}

void ASGameModeBase::StartPlay() {
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
	float tempSum = 0;
	for (int i = 0; i < PowerUpsToSpawn.Num(); i++) {
		tempSum += PowerUpsToSpawn[i].weight;
	}
	SumOfBuffWeights = tempSum;
	GetWorldTimerManager().SetTimer(TimerHandle_PowerUps, this, &ASGameModeBase::SpawnPowerUpTimerElapsed, PowerUpsTimerInterval, true);
}

void ASGameModeBase::KillAllAI()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) {
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (AttributeComp && AttributeComp->IsAlive()) {
			AttributeComp->Kill(this); //@todo Pass in player for kill credits?
		}
	}
}

void ASGameModeBase::SpawnBotTimerElapsed()
{

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance)) {
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnBotQueryCompleted);
	}
}

void ASGameModeBase::OnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus) {
	
	if (!CVarSpawnBots.GetValueOnGameThread()) {
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled by CVarSpawnBots"))
		return;
	}

	if (QueryStatus != EEnvQueryStatus::Success) {
		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot EQS Query Failed!"));
		return;
	}

	int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) {
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (AttributeComp && AttributeComp->IsAlive()) {
			NrOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), NrOfAliveBots);

	float MaxBotCount = 10.0f;

	if (DifficultyCurve) {
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NrOfAliveBots >= MaxBotCount) {
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping bot spawn."));
		return;
	}


	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.Num() > 0) {
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* KillerActor)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if (Player) {
		FTimerHandle TimerHandle_RespawnTimer;
		FTimerDelegate Delegate;

		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, Delegate, RespawnDelay, false);
	}

	ASCharacter* PlayerKiller = Cast<ASCharacter>(KillerActor);
	if (PlayerKiller) {
		UE_LOG(LogTemp, Log, TEXT("Added Kill Credits!"));
		ASPlayerState* PS = Cast<ASPlayerState>(PlayerKiller->GetPlayerState());
		PS->ApplyCreditsChange(CreditPointsForKill);
	}
}

void ASGameModeBase::RespawnPlayerElapsed(AController* PlayerController)
{
	if (ensure(PlayerController)) {
		UE_LOG(LogTemp, Log, TEXT("Respawning Player"));
		PlayerController->UnPossess();
		RestartPlayer(PlayerController);
	}
}

int ASGameModeBase::PickRandomPowerUpIndex()
{
	int index = -1;
	float rnd = FMath::RandRange(0.0f, SumOfBuffWeights);
	for (int i = 0; i < PowerUpsToSpawn.Num(); i++) {
		float item_weight = PowerUpsToSpawn[i].weight;
		if (rnd < item_weight) {
			index = i;
			break;
		}
		rnd -= item_weight;
	}

	check(index >= 0)
	return index;
}


void ASGameModeBase::SpawnPowerUpTimerElapsed()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBuffsQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance)) {
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnPowerUpsQueryCompleted);
	}
}

void ASGameModeBase::OnPowerUpsQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus) {
	if (!CVarSpawnPowerUps.GetValueOnGameThread()) {
		UE_LOG(LogTemp, Warning, TEXT("PowerUps spawning disabled by CVarSpawnPowerUps"));
	}

	if (QueryStatus != EEnvQueryStatus::Success) {
		UE_LOG(LogTemp, Warning, TEXT("Spawn PowerUp EQS Query Failed!"));
		return;
	}


	TArray<AActor*> FoundPowerUps;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASPowerUpBase::StaticClass(), FoundPowerUps);
	float MinBuffs, MaxBuffs;
	BuffAmount->GetValueRange(MinBuffs, MaxBuffs);

	if (FoundPowerUps.Num() >= (int)MaxBuffs) {
		UE_LOG(LogTemp, Warning, TEXT("Finished PowerUps spawning, clearing timer"));
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerUps);
		return;
	}

	if (FoundPowerUps.Num() < BuffAmount->GetFloatValue(GetWorld()->TimeSeconds)) {
		int DrawPowerUpIndex = PickRandomPowerUpIndex();
		UE_LOG(LogTemp, Log, TEXT("Picked power up with index %d"), DrawPowerUpIndex);
		TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
		if (Locations.Num() > 0) {
			FVector PowerUpLocation = Locations[0];
			PowerUpLocation.Z = 90.0f;
			GetWorld()->SpawnActor<AActor>(PowerUpsToSpawn[DrawPowerUpIndex].powerUp, PowerUpLocation, FRotator::ZeroRotator);
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("At maximum current PowerUps capacity. Skipping spawn."));
}
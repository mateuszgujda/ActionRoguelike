// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"


static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component "), ECVF_Cheat);

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	// ...
	HealthMax = 100.0f;
	Health = HealthMax;

	RageMax = 100.0f;
	Rage = 0;
	RageFromDamagePercentile = 0.2f;

	SetIsReplicatedByDefault(true);
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, HealthMax);
	DOREPLIFETIME(USAttributeComponent, Rage);
	DOREPLIFETIME(USAttributeComponent, RageMax);
}


void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void USAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}



bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -HealthMax);
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float DeltaHealth)
{
	if (!GetOwner()->CanBeDamaged()) {
		return false;
	}

	if (DeltaHealth < 0.0f) {
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		DeltaHealth *= DamageMultiplier;
	}

	const float NewHealth = FMath::Clamp<float>(Health + DeltaHealth, 0.0f, HealthMax);
	const float ProperDelta = NewHealth - Health;
	
	//Is Server?
	if (GetOwner()->HasAuthority()) {
		Health = NewHealth;
		if (ProperDelta != 0) {
			//OnHealthChanged.Broadcast(InstigatorActor, this, Health, ProperDelta);
			MulticastHealthChanged(InstigatorActor, Health, ProperDelta);
		}

		if (ProperDelta < 0.0f && Health <= 0.0f) {
			ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
			if (GM) {
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}

	return ProperDelta != 0;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::IsMaxHealth() const
{
	return HealthMax == Health;
}

float USAttributeComponent::GetCurrentHealth() const {
	return Health;
}

float USAttributeComponent::GetMaxHealth() const {
	return HealthMax;
}

float USAttributeComponent::GetCurrentRage() const
{
	return Rage;
}

float USAttributeComponent::GetMaxRage() const
{
	return RageMax;
}

float USAttributeComponent::GetRageFromDamagePercentile() const
{
	return RageFromDamagePercentile;
}

bool USAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float DeltaRage) {
	const float NewRage = FMath::Clamp<float>(Rage + DeltaRage, 0.0f, RageMax);
	const float ProperDelta = NewRage - Rage;
	Rage = NewRage;
	if (ProperDelta != 0) {
		
		//OnRageChanged.Broadcast(InstigatorActor, this, Rage, ProperDelta);
		MulticastRageChanged(InstigatorActor, Rage, ProperDelta);
	}

	return ProperDelta != 0;
}


USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor) {
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* Attrs = GetAttributes(Actor);
	if (Attrs) {
		return Attrs->IsAlive();
	}
	return false;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	// ...
	HealthMax = 100.0f;
	Health = HealthMax;
}


bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float DeltaHealth)
{
	const float NewHealth = FMath::Clamp<float>(Health + DeltaHealth, 0.0f, HealthMax);
	const float ProperDelta = NewHealth - Health;
	Health = NewHealth;
	
	if (ProperDelta != 0) {
		OnHealthChanged.Broadcast(InstigatorActor, this, Health, ProperDelta);

		return true;
	}
	return false;
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

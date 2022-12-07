// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	// ...
	HealthMax = 100.0f;
	Health = HealthMax;
}


bool USAttributeComponent::ApplyHealthChange(float DeltaHealth)
{
	const float NewHealth = FMath::Clamp<float>(Health + DeltaHealth, 0.0f, HealthMax);
	const float ProperDelta = NewHealth - Health;
	Health = NewHealth;
	
	if (ProperDelta != 0) {
		OnHealthChanged.Broadcast(nullptr, this, Health, ProperDelta);

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


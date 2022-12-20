// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect_Thorns.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"

USActionEffect_Thorns::USActionEffect_Thorns()
{
	Period = 0.0f;
	Duration = 0.0f;
	ReflectionModifier = 0.2f;
}

void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes) {
		Attributes->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
	USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes) {
		Attributes->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::OnHealthChanged(AActor* ChangeInstigator, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* Owner = GetOwningComponent()->GetOwner();
	if (ChangeInstigator != Owner && Delta < 0) {

		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectionModifier);
		if (ReflectedAmount != 0) {
			USAttributeComponent* AttackerAttributes = USAttributeComponent::GetAttributes(ChangeInstigator);
			if (AttackerAttributes) {
				AttackerAttributes->ApplyHealthChange(Owner, ReflectedAmount);
			}
		}

	}
}

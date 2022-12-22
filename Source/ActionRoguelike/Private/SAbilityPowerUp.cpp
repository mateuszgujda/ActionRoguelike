// Fill out your copyright notice in the Description page of Project Settings.


#include "SAbilityPowerUp.h"
#include "SActionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"

void ASAbilityPowerUp::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn && ActionToGrant)) {
		return;
	}
	USActionComponent* ActionComp = USActionComponent::GetActions(InstigatorPawn);
	if (!ActionComp) {
		return;
	}

	if (ActionComp->GetAction(ActionToGrant)) {
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Blue, TEXT("Already known ability"));
		return;
	}

	ActionComp->AddAction(InstigatorPawn, ActionToGrant);
	StartRecharge();
}

void ASAbilityPowerUp::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	RootComp->SetScalarParameterValueOnMaterials("RechargeDuration", RechargeTime);
}

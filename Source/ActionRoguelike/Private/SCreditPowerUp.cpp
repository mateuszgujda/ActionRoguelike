// Fill out your copyright notice in the Description page of Project Settings.


#include "SCreditPowerUp.h"
#include "SPlayerState.h"

void ASCreditPowerUp::Interact_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn) {
		ASPlayerState* PS = Cast<ASPlayerState>(InstigatorPawn->GetPlayerState());
		if (PS) {
			PS->ApplyCreditsChange(BoostValue);

			StartRecharge();
		}
	}
}

void ASCreditPowerUp::StartRecharge() {
	RootComp->SetScalarParameterValueOnMaterials("ConsumeTime", GetWorld()->TimeSeconds);
	Super::StartRecharge();
}

void ASCreditPowerUp::EndRecharge() {
	Super::EndRecharge();
}

void ASCreditPowerUp::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	RootComp->SetScalarParameterValueOnMaterials("RechargeDuration", RechargeTime);
}

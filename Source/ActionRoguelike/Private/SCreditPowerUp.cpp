// Fill out your copyright notice in the Description page of Project Settings.


#include "SCreditPowerUp.h"
#include "SPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"

void ASCreditPowerUp::Interact_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn) {
		ASPlayerState* PS = Cast<ASPlayerState>(InstigatorPawn->GetPlayerState());
		if (PS) {
			if (HasAuthority()) {
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, "PlayerState After Server ");
			}
			else {
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, "PlayerState After Client");
			}
			PS->ApplyCreditsChange(BoostValue);

			StartRecharge();
		}
	}
}

void ASCreditPowerUp::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	RootComp->SetScalarParameterValueOnMaterials("RechargeDuration", RechargeTime);
}


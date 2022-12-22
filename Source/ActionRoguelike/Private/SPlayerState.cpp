// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "Net/UnrealNetwork.h"


ASPlayerState::ASPlayerState()
{
	Credits = 0.0f;
	SetReplicates(true);
}

void ASPlayerState::ApplyCreditsChange(float DeltaCredits)
{
	//if (GetOwner()) {
	//	if (GetOwner()->HasAuthority()) {
	//		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, "Credits Changed serv: " + FString::SanitizeFloat(this->Credits));
	//	}
	//	else {
	//		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, "Credits Changed client: " + FString::SanitizeFloat(this->Credits));
	//	}
	//}
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, "Credits Changed serv: " + FString::SanitizeFloat(this->Credits));
	Credits += DeltaCredits;
	OnCreditsChanged.Broadcast(this, Credits, DeltaCredits);
	//OnRep_Credits(Credits);
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, Credits);

}


float ASPlayerState::GetCredits() const
{
	return Credits;
}

void ASPlayerState::OnRep_Credits(float OldCredits)
{

	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

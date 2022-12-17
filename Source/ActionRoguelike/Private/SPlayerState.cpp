// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"



ASPlayerState::ASPlayerState()
{
	Credits = 0.0f;
}

void ASPlayerState::ApplyCreditsChange(float DeltaCredits)
{
	Credits += DeltaCredits;

	OnCreditsChanged.Broadcast(Credits);
}

float ASPlayerState::GetCredits() const
{
	return Credits;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/SAIStatics.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);

	//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	//if (PlayerPawn) {
	//	GetBlackboardComponent()->SetValueAsVector(USAIStatics::GetAIMoveToLocationKeyName(), PlayerPawn->GetActorLocation());

	//	GetBlackboardComponent()->SetValueAsObject(USAIStatics::GetAITargetActorKeyName(), PlayerPawn);
	//}
}

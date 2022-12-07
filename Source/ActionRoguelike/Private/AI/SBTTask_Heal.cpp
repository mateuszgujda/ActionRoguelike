// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_Heal.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (ensure(AIController)) {
		APawn* Pawn = AIController->GetPawn();
		USAttributeComponent* AttrComp = Cast<USAttributeComponent>(Pawn->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttrComp == nullptr) {
			return EBTNodeResult::Failed;
		}

		AttrComp->ApplyHealthChange(AttrComp->GetMaxHealth() - AttrComp->GetCurrentHealth());

		return AttrComp->IsMaxHealth() ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}
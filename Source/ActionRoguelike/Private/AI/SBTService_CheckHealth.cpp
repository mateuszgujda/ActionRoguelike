// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckHealth.h"
#include "AI/SAIStatics.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


void USBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensureMsgf(BlackboardComp, TEXT("BlackboardComponent is NULL!"))) {
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ensure(MyController)) {
			APawn* AIPawn = MyController->GetPawn();
			if (ensure(AIPawn)) {
				USAttributeComponent* AttrComp = Cast<USAttributeComponent>(AIPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
				if (AttrComp) {
					const float CurrentHealth = AttrComp->GetCurrentHealth();
					BlackboardComp->SetValueAsBool(USAIStatics::GetAIIsOnLowHealthKeyName(), CurrentHealth <= LowHealthHitpoints);
				}
			}
		}
	}
}
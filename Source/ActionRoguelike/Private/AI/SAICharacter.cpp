// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "AI/SAIStatics.h"
#include "SAttributeComponent.h"
// Sets default values
ASAICharacter::ASAICharacter()
{

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSesingComp");
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
}

void ASAICharacter::OnPawnSeen(APawn* Pawn) {
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC) {
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();
		BBComp->SetValueAsObject(USAIStatics::GetAITargetActorKeyName(), Pawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "Player Spotted", nullptr, FColor::White, 4.0f, true);
	}
}
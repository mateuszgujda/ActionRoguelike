// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "AI/SAIStatics.h"
#include "BrainComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/SWorldUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "SAttributeComponent.h"
// Sets default values
ASAICharacter::ASAICharacter()
{

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSesingComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	HitFlashColor = FColor::Red;
	HealFlashColor = FColor::Green;
	HitFlashSpeed = 5.0f;
	ActiveHealthBar = nullptr;
}

void ASAICharacter::OnPawnSeen(APawn* Pawn) {
	SetTargetActor(Pawn);
	DrawDebugString(GetWorld(), GetActorLocation(), "Player Spotted", nullptr, FColor::White, 4.0f, true);
}

void ASAICharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);

	UMeshComponent* MainMesh = GetMesh();
	if (MainMesh) {
		MainMesh->SetScalarParameterValueOnMaterials("HitFlashSpeed", HitFlashSpeed);
		MainMesh->SetVectorParameterValueOnMaterials("HitFlashColor", FVector(HitFlashColor.R, HitFlashColor.G, HitFlashColor.B));
	}
}

void ASAICharacter::OnHealthChanged(AActor* ChangeInstigator, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (!ActiveHealthBar) {
		ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
		if (ActiveHealthBar) {
			ActiveHealthBar->SetAttachedActor(this);
			ActiveHealthBar->AddToViewport();
		}
	}
	if (Delta < 0) {
		if (ChangeInstigator != this) {
			SetTargetActor(ChangeInstigator);
		}

		if (NewHealth <= 0) {
			AAIController* Control = Cast<AAIController>(GetController());
			if (Control) {
				Control->GetBrainComponent()->StopLogic("Killed");
			}

			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			SetLifeSpan(10.0f);
		}
	}

	USkeletalMeshComponent* MeshComp = GetMesh();
	if (MeshComp) {
		if (Delta <= 0) {
			MeshComp->SetVectorParameterValueOnMaterials("HitFlashColor", FVector(HitFlashColor.R, HitFlashColor.G, HitFlashColor.B));
		}
		else {
			MeshComp->SetVectorParameterValueOnMaterials("HitFlashColor", FVector(HealFlashColor.R, HealFlashColor.G, HealFlashColor.B));
		}
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}

void ASAICharacter::SetTargetActor(AActor* Target) {
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC) {
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();
		BBComp->SetValueAsObject(USAIStatics::GetAITargetActorKeyName(), Target);
	}
}
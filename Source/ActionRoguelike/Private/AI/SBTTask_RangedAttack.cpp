// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "SAttributeComponent.h"
#include "AI/SAIStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (ensure(AIController)) {
		ACharacter* Character = Cast<ACharacter>(AIController->GetPawn());
		if (Character == nullptr) {
			return EBTNodeResult::Failed;
		}

		FVector MuzzleLocation = Character->GetMesh()->GetSocketLocation(USAIStatics::GetAICharacterMuzzleName());

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(USAIStatics::GetAITargetActorKeyName()));

		if (TargetActor == nullptr) {
			return EBTNodeResult::Failed;
		}

		if (!USAttributeComponent::IsActorAlive(TargetActor)) {
			return EBTNodeResult::Failed;
		}

		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		MuzzleRotation.Pitch += FMath::RandRange(0.0f, BulletMaxSpread);
		MuzzleRotation.Yaw += FMath::RandRange(-BulletMaxSpread, BulletMaxSpread);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = Character;

		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}

USBTTask_RangedAttack::USBTTask_RangedAttack()
{
	BulletMaxSpread = 2.0f;
}

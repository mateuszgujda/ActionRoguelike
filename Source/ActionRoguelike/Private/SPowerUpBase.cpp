// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUpBase.h"
#include "Kismet/GameplayStatics.h"
#include "SPlayerState.h"
#include "SAttributeComponent.h"

// Sets default values
ASPowerUpBase::ASPowerUpBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoostValue = 20.0f;
	RechargeTime = 10.0f;
	InteractionCost = 0.0f;
	RootComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(RootComp);
}

void ASPowerUpBase::Interact_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn) {
		ASPlayerState* PS = Cast<ASPlayerState>(InstigatorPawn->GetPlayerState());
		if (PS && PS->GetCredits() >= InteractionCost) {
			USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
			if (AttributeComp && !AttributeComp->IsMaxHealth()) {
				if (AttributeComp->ApplyHealthChange(this, BoostValue)) {
					PS->ApplyCreditsChange(-InteractionCost);
				}
				//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, GetActorLocation(), GetActorRotation());

				StartRecharge();
			}
		}
	}
}

// Called when the game starts or when spawned
void ASPowerUpBase::BeginPlay()
{
	Super::BeginPlay();
	DefaultCollision = RootComp->GetCollisionEnabled();
}

void ASPowerUpBase::StartRecharge()
{
	bIsRecharging = true;
	RootComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorldTimerManager().SetTimer(RechargeTimeHandle, this, &ASPowerUpBase::EndRecharge, RechargeTime);
}

void ASPowerUpBase::EndRecharge()
{
	GetWorldTimerManager().ClearTimer(RechargeTimeHandle);
	bIsRecharging = false;
	RootComp->SetCollisionEnabled(DefaultCollision);
}

// Called every frame
void ASPowerUpBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


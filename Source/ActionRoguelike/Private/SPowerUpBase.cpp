// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUpBase.h"
#include "Kismet/GameplayStatics.h"
#include "SPlayerState.h"
#include "SAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"

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
	SetReplicates(true);
}

void ASPowerUpBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPowerUpBase, bIsRecharging);
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

void ASPowerUpBase::OnRep_RechargeValueChange()
{
	if (bIsRecharging) {

		RootComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RootComp->SetScalarParameterValueOnMaterials("ConsumeTime", UGameplayStatics::GetGameState(GetWorld())->GetServerWorldTimeSeconds());
		//if (HasAuthority()) {
		//	GetWorldTimerManager().SetTimer(RechargeTimeHandle, this, &ASPowerUpBase::EndRecharge, RechargeTime);
		//	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, "EnableServer ");
		//}
		//else {
		//	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, "EnableClient" );
		//}
	}
	else {

		//if (HasAuthority()) {
		//	GetWorldTimerManager().ClearTimer(RechargeTimeHandle);
		//	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, "DisableServer ");
		//}
		//else {
		//	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, "DisableClient" );
		//}
		RootComp->SetScalarParameterValueOnMaterials("ConsumeTime", 0.0f);
		RootComp->SetCollisionEnabled(DefaultCollision);
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
	OnRep_RechargeValueChange();

}

void ASPowerUpBase::EndRecharge()
{

	bIsRecharging = false;
	OnRep_RechargeValueChange();
}

// Called every frame
void ASPowerUpBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


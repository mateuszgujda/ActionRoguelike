// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ASDashProjectile::ASDashProjectile() {
	ProjectileLifetime = 0.2f;
	TeleportOffset = 0.2f;
	
	MovementComp->InitialSpeed = 6000.0f;
}

void ASDashProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	GetWorldTimerManager().ClearTimer(SpawnLifetime_Handle);
	Explode();
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(SpawnLifetime_Handle, this, &ASDashProjectile::Explode, ProjectileLifetime);
}

void ASDashProjectile::Explode_Implementation() {
	GetWorldTimerManager().ClearTimer(SpawnLifetime_Handle);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, GetActorLocation(), GetActorRotation());

	EffectComp->DeactivateSystem();
	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle Teleport_Handle;
	GetWorldTimerManager().SetTimer(Teleport_Handle, this, &ASDashProjectile::TeleportInstigator, TeleportOffset);
}

void ASDashProjectile::TeleportInstigator() {
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport)) {
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
	}
}
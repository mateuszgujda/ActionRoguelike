// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "SAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SGameplayFunctionLibrary.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComp->AttachTo(RootComponent);
	DeltaHealthAmount = -20.0f;
}

void ASMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != GetInstigator()) {
		USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DeltaHealthAmount, Hit);
		Explode(OtherActor);
	}
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	AActor* Inst = GetInstigator();
	if (Inst) {
		SphereComp->IgnoreActorWhenMoving(Inst, true);
	}


	AudioComp->Play();
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor && OtherActor != GetInstigator()) {
		USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DeltaHealthAmount, SweepResult);
		Explode(OtherActor);
	}
}


void ASMagicProjectile::Explode(AActor* OtherActor) {
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, GetActorLocation(), GetActorRotation());
	if (ImpactSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
	}
	if (ImpactShake) {
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), ImpactShake, OtherActor->GetActorLocation(), 0.0f, 1000.0f);
	}
	Destroy();
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
	SphereComp->OnComponentHit.AddDynamic(this, &ASMagicProjectile::OnActorHit);
}

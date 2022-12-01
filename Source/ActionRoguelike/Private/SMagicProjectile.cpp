// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "SAttributeComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComp->AttachTo(RootComponent);
}

void ASMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode(OtherActor);
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
	Explode(OtherActor);
}


void ASMagicProjectile::Explode(AActor* OtherActor) {
	if (OtherActor && OtherActor != GetInstigator()) {


		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp) {
			AttributeComp->ApplyHealthChange(-20.0f);
			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, GetActorLocation(), GetActorRotation());
		}

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, GetActorLocation(), GetActorRotation());
		if (ImpactSound) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
		}

		Destroy();
	}
}
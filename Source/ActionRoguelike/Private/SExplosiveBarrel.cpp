// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMeshComp;
	StaticMeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnHit);
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForceComp->SetupAttachment(StaticMeshComp);

	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->Radius = 800.0f;
	RadialForceComp->ImpulseStrength = 2000.0f;
	RadialForceComp->DestructibleDamage = 500.0f;
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASExplosiveBarrel::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL)) {
		RadialForceComp->FireImpulse();
	}
}

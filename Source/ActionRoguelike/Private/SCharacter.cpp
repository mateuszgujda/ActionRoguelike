// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComponent"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ASCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("DashAction", IE_Pressed, this, &ASCharacter::DashAction);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
}

void ASCharacter::MoveForward(float value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	AddMovementInput(ControlRotation.Vector(), value);
}

void ASCharacter::MoveRight(float value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	AddMovementInput(UKismetMathLibrary::GetRightVector(ControlRotation), value);
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);

}

void ASCharacter::SecondaryAttack()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_SecondaryAttack, this, &ASCharacter::SecondaryAttack_TimeElapsed, 0.2f);

}

void ASCharacter::DashAction() {
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_DashAction, this, &ASCharacter::DashAction_TimeElapsed, 0.2f);
}

void ASCharacter::PrimaryInteract() {
	if (InteractionComp) {
		InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::DashAction_TimeElapsed() {
	FireProjectile(DashProjectileClass);
}

void ASCharacter::SecondaryAttack_TimeElapsed() {
	FireProjectile(SecondaryProjectileClass);
}

void ASCharacter::PrimaryAttack_TimeElapsed() {
	FireProjectile(PrimaryProjectileClass);
}


void ASCharacter::FireProjectile(TSubclassOf<ASProjectile> ProjectileClass) {
	FVector HandSocketLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FHitResult HitResult;
	FVector Start = CameraComp->GetComponentLocation();
	FVector End = Start + GetControlRotation().Vector() * 5000.0f;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);


	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, ObjectQueryParams);
	FRotator LookingRotation;
	FColor DebugColor;
	if (bBlockingHit) {
		LookingRotation = UKismetMathLibrary::FindLookAtRotation(HandSocketLocation, HitResult.ImpactPoint);
		DebugColor = FColor::Green;
	}
	else {
		LookingRotation = UKismetMathLibrary::FindLookAtRotation(HandSocketLocation, End);
		DebugColor = FColor::Red;
	}
	DrawDebugLine(GetWorld(), Start, End, DebugColor, false, 2.0f, 0, 2.0f);
	FTransform SpawnTM = FTransform(LookingRotation, HandSocketLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	AActor* SpawnedProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	UCapsuleComponent* PlayerCapsule = GetCapsuleComponent();
	PlayerCapsule->IgnoreActorWhenMoving(SpawnedProjectile, true);
}


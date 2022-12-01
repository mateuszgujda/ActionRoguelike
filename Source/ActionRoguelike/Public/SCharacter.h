// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SProjectile.h"
#include "SCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class UAnimMontage;
class USAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ASProjectile> PrimaryProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ASProjectile> SecondaryProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ASProjectile> DashProjectileClass;



	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_SecondaryAttack;
	FTimerHandle TimerHandle_DashAction;


public:
	// Sets default values for this character's properties
	ASCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USAttributeComponent* AttributeComp;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitFlash")
	FColor HitFlashColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitFlash")
	float HitFlashSpeed;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();
	void SecondaryAttack();
	void SecondaryAttack_TimeElapsed();
	void DashAction();
	void DashAction_TimeElapsed();
	void FireProjectile(TSubclassOf<ASProjectile> Projectile);
	void PrimaryInteract();
	
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnHealthChanged(AActor* ChangeInstigator, USAttributeComponent* OwningComp, float NewHealth, float Delta);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

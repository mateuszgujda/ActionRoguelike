// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"


class UPawnSensingComponent;
class USAttributeComponent;
class USWorldUserWidget;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:

	USWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(VisibleAnywhere, Category="AI Senses")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USAttributeComponent* AttributeComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitFlash")
	FColor HitFlashColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitFlash")
	FColor HealFlashColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitFlash")
	float HitFlashSpeed;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* ChangeInstigator, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	void PostInitializeComponents() override;

	void SetTargetActor(AActor* Target);
};

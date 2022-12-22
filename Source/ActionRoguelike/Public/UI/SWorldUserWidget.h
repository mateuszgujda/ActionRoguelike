// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.generated.h"

class USizeBox;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(meta=(BindWidget))
	USizeBox* ParentSizeBox; 

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetAttachedActor(AActor* Actor);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FVector WorldOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (ExposeOnSpawn=true))
	AActor* AttachedActor;
};

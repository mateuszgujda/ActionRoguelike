// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

void ASHealthPotion::StartRecharge()
{
	RootComp->SetScalarParameterValueOnMaterials("ConsumeTime", GetWorld()->TimeSeconds);
	Super::StartRecharge();
	
}

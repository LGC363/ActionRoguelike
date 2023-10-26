// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	Health = 100.0;

}


bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;
	UE_LOG(LogTemp, Warning, TEXT("ApplyHealthChange"));
	return true;
}

float USAttributeComponent::GetHealth()
{
	return this->Health;
}




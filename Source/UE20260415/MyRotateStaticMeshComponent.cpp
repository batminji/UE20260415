// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRotateStaticMeshComponent.h"

UMyRotateStaticMeshComponent::UMyRotateStaticMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = true;


}

void UMyRotateStaticMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AddLocalRotation(FRotator(0.0f, 0.0f, RotationSpeed * DeltaTime));
}

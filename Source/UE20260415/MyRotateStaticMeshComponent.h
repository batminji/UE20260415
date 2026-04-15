// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "MyRotateStaticMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class UE20260415_API UMyRotateStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UMyRotateStaticMeshComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float RotationSpeed = 720.0f;
	
};

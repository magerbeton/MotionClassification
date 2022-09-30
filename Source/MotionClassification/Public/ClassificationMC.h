// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "ClassificationMC.generated.h"

/**
 * 
 */
UCLASS()
class MOTIONCLASSIFICATION_API UClassificationMC : public UMotionControllerComponent
{
	GENERATED_BODY()

public:

	UClassificationMC();

	UFUNCTION(BlueprintCallable, Category="Logging")
	void GetControllerInfo();

	UPROPERTY()
	FVector LastHandLocation;

	UPROPERTY()
	float Time;

	UPROPERTY()
	int TargetSamples;
	
	UPROPERTY()
	float TargetSampleRate;

	UFUNCTION(BlueprintCallable, Category="Testing")
	void TestInterpolation(FVector CurrentHandLoc, FVector newLastHandLoc, float newTime, float newDeltaTime);
	
protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};

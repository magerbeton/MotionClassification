// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VR_DefaultChar.generated.h"

class UCameraComponent; class UClassificationMC; class UFastClassificationMC;

UCLASS()
class MOTIONCLASSIFICATION_API AVR_DefaultChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVR_DefaultChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* CameraRoot;

	UPROPERTY(BlueprintReadWrite)
	UCameraComponent* PlayerCamera;

	//----------------------------------------------
	// Hand Right

	/*
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Hand")
	UClassificationMC* MotionControllerR;
	*/

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Hand")
	UFastClassificationMC* MotionControllerR;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Hand")
	UStaticMeshComponent* HandMeshR;

	//----------------------------------------------
	// Hand Left

	/*
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Hand")
	UClassificationMC* MotionControllerL;
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Hand")
	UFastClassificationMC* MotionControllerL;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Hand")
	UStaticMeshComponent* HandMeshL;

	//----------------------------------------------
	//Debug
	UFUNCTION()
	void Test();
	
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "VR_DefaultChar.h"

#include "ClassificationMC.h"
#include "FastClassificationMC.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "XRMotionControllerBase.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AVR_DefaultChar::AVR_DefaultChar()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	CameraRoot->SetupAttachment(Cast<USceneComponent>(GetCapsuleComponent()));

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	PlayerCamera->SetupAttachment(CameraRoot);

	//----------------------------------------------
	// Hand Left
	MotionControllerL = CreateDefaultSubobject<UClassificationMC>(TEXT("MotionControllerL"));
	MotionControllerL->SetupAttachment(CameraRoot);
	MotionControllerL->MotionSource = FXRMotionControllerBase::LeftHandSourceId;

	HandMeshL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandMeshL"));
	HandMeshL->SetupAttachment(MotionControllerL);
	/*const ConstructorHelpers::FObjectFinder<UStaticMesh>HandMeshL_Obj(TEXT(""));
	if(HandMeshL_Obj.Succeeded())
	{
		HandMeshL->SetStaticMesh(HandMeshL_Obj.Object);
	}*/
	
	//----------------------------------------------
	// Hand Right
	MotionControllerR = CreateDefaultSubobject<UFastClassificationMC>(TEXT("MotionControllerR"));
	//MotionControllerR = CreateDefaultSubobject<UClassificationMC>(TEXT("MotionControllerR"));
	MotionControllerR->SetupAttachment(CameraRoot);
	MotionControllerR->MotionSource = FXRMotionControllerBase::RightHandSourceId;

	HandMeshR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandMeshR"));
	HandMeshR->SetupAttachment(CameraRoot);
	/*const ConstructorHelpers::FObjectFinder<UStaticMesh>HandMeshR_Obj(TEXT(""));
	if(HandMeshR_Obj.Succeeded())
	{
		HandMeshR->SetStaticMesh(HandMeshR_Obj.Object);
	}*/

	//----------------------------------------------
	// Default variable inits
	
}

// Called when the game starts or when spawned
void AVR_DefaultChar::BeginPlay()
{
	Super::BeginPlay();

	//InputComponent->BindAction("Debug",EInputEvent::IE_Pressed,this,&AVR_DefaultChar::Test);
}

void AVR_DefaultChar::Test()
{
	//MotionControllerL->TestInterpolation(FVector(1.0f,0.0f,0.0f),FVector(1.0f,0.0f,0.0f),0.5f,2.5f);
	
	TArray<FVector> s;
	TArray<FVector> t;

	for (int i = 0; i < 32; ++i)
	{
		s[i] = FVector(i,i,i);
		t[i] = FVector(i/2,i/2,i/2);
	}
	
	uint32** Arr = MotionControllerR->DtwDistance(s,t);
	for (int i = 0; i < 32; ++i)
	{
		FString Result = "";
		for (int j = 0; j < 32; ++j)
		{
			Result.Append(FString::Printf(TEXT("%i,"),*static_cast<int>(Arr[i])));
			//UE_LOG(LogTemp,Display,TEXT(""));
		}
		UE_LOG(LogTemp,Display,TEXT("Text: %s"),*Result);
	}
}


// Called every frame
void AVR_DefaultChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

// Called to bind functionality to input
void AVR_DefaultChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


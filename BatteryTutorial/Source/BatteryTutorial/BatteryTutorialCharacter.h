// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

class USpringArmComponent;
#include "BatteryTutorialCharacter.generated.h"

UCLASS(config=Game)
class ABatteryTutorialCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TSubobjectPtr<class USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TSubobjectPtr<class UCameraComponent> FollowCamera;

	/** Collection volume for power objects */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Power)
	TSubobjectPtr<class USphereComponent> CollectionSphere;

	/** The current power level of the object */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Power)
	float PowerLevel;

	/** amount of speed gained based on powerlevel */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Power)
	float SpeedFactor;

	/** Baseline speed of the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Power)
	float BaseSpeed;

	/** ontick */
	virtual void Tick(float DeltaSeconds) OVERRIDE;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** called when we press the collectpickups button, collects all batteries in the pickup sphere */
	UFUNCTION(BlueprintCallable, Category = Power)
	void CollectBatteries();

	/** add to power level */
	UFUNCTION(BlueprintImplementableEvent, Category = Power)
	void PowerUp(float PowerLevelDelta);

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) OVERRIDE;
	// End of APawn interface
};


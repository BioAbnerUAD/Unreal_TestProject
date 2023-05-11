// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GasCharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGasCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREAL_TEST_PROJECT_API IGasCharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Animation Interface")
	void TriggerSaveAttack();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Animation Interface")
	void TriggerResetCombo();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UI Interface")
	void GetHealthValuesUI(float& Health, float& MaxHealth);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UI Interface")
	void GetManaValuesUI(float& Mana, float& MaxMana);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ability Interface")
	void GetProjectileTransforms(FTransform& SpawnTransforms);
};

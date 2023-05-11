// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASGameplayAbility.h"
#include "GAProjectile.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_TEST_PROJECT_API UGAProjectile : public UGASGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
															 const FGameplayAbilityActorInfo* ActorInfo,
													     const FGameplayAbilityActivationInfo ActivationInfo, 
															 const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnCompleted();

	UFUNCTION()
	void OnEventRecieved(FGameplayEventData Payload);
	
};

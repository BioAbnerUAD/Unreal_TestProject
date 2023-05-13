// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASGameplayAbility.h"
#include "GATornSpace.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_TEST_PROJECT_API UGATornSpace : public UGASGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
															 const FGameplayAbilityActorInfo* ActorInfo,
													     const FGameplayAbilityActivationInfo ActivationInfo, 
															 const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnValidData(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION()
	void OnCancelled(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION()
	void OnTargetReached();

	void Native_OnEndAbility_Implementation(bool bWasCancelled) override;
	
};

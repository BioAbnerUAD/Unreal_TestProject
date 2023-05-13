// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GetAbilityInfo.h"
#include "GASGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_TEST_PROJECT_API UGASGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	void EndAbility(const FGameplayAbilitySpecHandle Handle,
									const FGameplayAbilityActorInfo* ActorInfo,
									const FGameplayAbilityActivationInfo ActivationInfo,
									bool bReplicateEndAbility,
									bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
	FGameplayAbilityInfo GetAbilityInfo();

	UFUNCTION(BlueprintNativeEvent, Category = "GASGameplayAbility")
	void Native_OnEndAbility(bool bWasCancelled);
	
};

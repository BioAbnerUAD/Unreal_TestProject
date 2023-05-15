// Fill out your copyright notice in the Description page of Project Settings.


#include "GAUltimate.h"

#include "BaseUltimate.h"
#include "DAPlayerGameplayAbilities.h"
#include "GasCharacterInterface.h"

void UGAUltimate::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
																	  const FGameplayAbilityActorInfo* ActorInfo,
																	  const FGameplayAbilityActivationInfo ActivationInfo,
																	  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!HasAuthority(&CurrentActivationInfo)) {
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
	}

	auto Owner = GetOwningActorFromActorInfo();
	auto AbilityData = Cast<IGasCharacterInterface>(Owner)->GetGameplayAbilityData_Implementation();

	FTransform UltimateTransform = Owner->GetActorTransform();

	if (auto Ultimate = GetWorld()->SpawnActorDeferred<ABaseUltimate>(AbilityData->BPUltimateClass, UltimateTransform)) {

		Ultimate->SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Ultimate->SetInstigator(Cast<APawn>(Owner));
		Ultimate->EffectSpec = MakeOutgoingGameplayEffectSpec(AbilityData->GEUltimateDamageClass).Data;

		Ultimate->FinishSpawning(UltimateTransform);
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

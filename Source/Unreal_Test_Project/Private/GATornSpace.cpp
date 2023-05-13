// Fill out your copyright notice in the Description page of Project Settings.


#include "GATornSpace.h"

#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Abilities/Tasks/AbilityTask_MoveToLocation.h"

#include "DAPlayerGameplayAbilities.h"
#include "GasCharacterInterface.h"
#include "GASTargetActor.h"

void UGATornSpace::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
																	  const FGameplayAbilityActorInfo* ActorInfo,
																	  const FGameplayAbilityActivationInfo ActivationInfo,
																	  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	auto Owner = Cast<IGasCharacterInterface>(GetOwningActorFromActorInfo());

	auto TargetActor = GetWorld()->SpawnActor<AGASTargetActor>();
	TargetActor->TraceRange = 1500.f;
	TargetActor->ReticleClass = Owner->GetGameplayAbilityData_Implementation()->GARetTornSpaceClass;

	auto WaitTask = UAbilityTask_WaitTargetData::WaitTargetDataUsingActor(
		this, 
		NAME_None,
		EGameplayTargetingConfirmation::UserConfirmed,
		TargetActor
	);

	WaitTask->ValidData.AddDynamic(this, &UGATornSpace::OnValidData);
	WaitTask->Cancelled.AddDynamic(this, &UGATornSpace::OnCancelled);

	WaitTask->ReadyForActivation();

	Owner->SetIsTargeting_Implementation(true);
}

void UGATornSpace::OnValidData(const FGameplayAbilityTargetDataHandle& Data)
{
	auto HitResults = Data.Data[0]->GetHitResult();
	auto TargetLocation = HitResults->bBlockingHit ? HitResults->Location : HitResults->TraceEnd;

	auto MoveTask = UAbilityTask_MoveToLocation::MoveToLocation(
		this,
		NAME_None,
		TargetLocation,
		0.2f,
		nullptr,
		nullptr
	);

	MoveTask->OnTargetLocationReached.AddDynamic(this, &UGATornSpace::OnTargetReached);
	MoveTask->ReadyForActivation();
}

void UGATornSpace::OnCancelled(const FGameplayAbilityTargetDataHandle& Data)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGATornSpace::OnTargetReached() {
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGATornSpace::Native_OnEndAbility_Implementation(bool bWasCancelled)
{
	Cast<IGasCharacterInterface>(GetOwningActorFromActorInfo())->SetIsTargeting_Implementation(false);
}

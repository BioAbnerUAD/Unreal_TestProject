// Fill out your copyright notice in the Description page of Project Settings.


#include "GACosmicRift.h"

#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Kismet/KismetSystemLibrary.h"

#include "BaseCosmicRiftMeteor.h"
#include "DAPlayerGameplayAbilities.h"
#include "GasCharacterInterface.h"
#include "GASTargetActor.h"

void UGACosmicRift::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
																	  const FGameplayAbilityActorInfo* ActorInfo,
																	  const FGameplayAbilityActivationInfo ActivationInfo,
																	  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	auto Owner = Cast<IGasCharacterInterface>(GetOwningActorFromActorInfo());

	auto TargetActor = GetWorld()->SpawnActor<AGASTargetActor>();
	TargetActor->TraceRange = 2500.f;
	TargetActor->ReticleClass = Owner->GetGameplayAbilityData_Implementation()->GARetCosmicRiftClass;

	auto WaitTask = UAbilityTask_WaitTargetData::WaitTargetDataUsingActor(
		this,
		NAME_None,
		EGameplayTargetingConfirmation::UserConfirmed,
		TargetActor
	);

	Owner->SetIsTargeting_Implementation(true);

	UKismetSystemLibrary::PrintString(GetWorld(), "Targeting Active");

	WaitTask->ValidData.AddDynamic(this, &UGACosmicRift::OnValidData);
	WaitTask->Cancelled.AddDynamic(this, &UGACosmicRift::OnCancelled);

	WaitTask->Activate();
}

void UGACosmicRift::OnValidData(const FGameplayAbilityTargetDataHandle& Data)
{
	auto Owner = Cast<IGasCharacterInterface>(GetOwningActorFromActorInfo());

	do {
		if (!HasAuthority(&CurrentActivationInfo)) { break; }

		auto HitResult = Data.Data[0].Get()->GetHitResult();
		if (!HitResult->bBlockingHit) {
			UKismetSystemLibrary::PrintString(GetWorld(), "Invalid Target");
			break; 
		}

		FTransform MeteorTransform = FTransform(FQuat::Identity, HitResult->Location, { 1, 1, 1 });

		if( auto Meteor = GetWorld()->SpawnActorDeferred<ABaseCosmicRiftMeteor>(Owner->GetGameplayAbilityData_Implementation()->BPMeteorClass, MeteorTransform) ) {

			Meteor->SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			Meteor->SetInstigator(Cast<APawn>(Owner));
			Meteor->EffectSpec = MakeOutgoingGameplayEffectSpec(Owner->GetGameplayAbilityData_Implementation()->GEMeteorDamageClass).Data;

			Meteor->FinishSpawning(MeteorTransform);
		} 

	} while (0);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGACosmicRift::OnCancelled(const FGameplayAbilityTargetDataHandle& Data)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGACosmicRift::Native_OnEndAbility_Implementation(bool bWasCancelled)
{
	Cast<IGasCharacterInterface>(GetOwningActorFromActorInfo())->SetIsTargeting_Implementation(false);
}

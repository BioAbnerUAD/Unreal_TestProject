// Fill out your copyright notice in the Description page of Project Settings.


#include "GADarkTether.h"

#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "GASCharacter.h"
#include "GASTargetActor.h"

void UGADarkTether::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
																	  const FGameplayAbilityActorInfo* ActorInfo,
																	  const FGameplayAbilityActivationInfo ActivationInfo,
																	  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	auto TargetActor = GetWorld()->SpawnActor<AGASTargetActor>();
	TargetActor->TraceRange = 2000.f;

	auto WaitTask = UAbilityTask_WaitTargetData::WaitTargetDataUsingActor(
		this,
		NAME_None, 
		EGameplayTargetingConfirmation::Instant,
		TargetActor
	);

	WaitTask->ValidData.AddDynamic(this, &UGADarkTether::OnValidData);
	WaitTask->Cancelled.AddDynamic(this, &UGADarkTether::OnCancelled);

	WaitTask->Activate();
}

void UGADarkTether::OnValidData(const FGameplayAbilityTargetDataHandle& Data)
{
	auto HitResult = Data.Data[0].Get()->GetHitResult();
	auto HitLocation = HitResult->bBlockingHit ? HitResult->Location : HitResult->TraceEnd;

	DrawDebugSphere(GetWorld(), HitLocation, 500.f, 12, FColor::Purple, false, 3.0f);
	
	TArray<AActor*> OverlappedActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), 
																						HitLocation, 
																						500.f, 
																						{ UEngineTypes::ConvertToObjectType(ECC_Pawn) },
																						nullptr, 
																						{ GetOwningActorFromActorInfo() }, 
																						OverlappedActors);

	for (auto Actor : OverlappedActors) {
		auto TargetCharacter = Cast<AGASCharacter>(Actor);
		if (!TargetCharacter) { break; }

		UAbilitySystemComponent* TargetASComp = TargetCharacter->GetAbilitySystemComponent();
		if (!TargetASComp) { break; }

		TargetASComp->ApplyGameplayEffectToSelf(
			TargetCharacter->GEDarkTetherSlowDownClass->GetDefaultObject<UGameplayEffect>(),
			0, 
			TargetASComp->MakeEffectContext()
		);

		UKismetSystemLibrary::PrintString(GetWorld(), "Effect Applied");
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

void UGADarkTether::OnCancelled(const FGameplayAbilityTargetDataHandle& Data)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "GAProjectile.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemComponent.h"

#include "BaseProjectile.h"
#include "GASCharacter.h"

void UGAProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
																	  const FGameplayAbilityActorInfo* ActorInfo,
																	  const FGameplayAbilityActivationInfo ActivationInfo,
																	  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	auto Owner = Cast<AGASCharacter>(ActorInfo->OwnerActor.Get());

	UAbilitySystemComponent* AbilityComponent = ActorInfo->AbilitySystemComponent.Get();
	UAnimMontage* Montage = Owner->PrimaryAttackAMontage;

	auto MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, Montage);
	MontageTask->OnCompleted.AddDynamic(this, &UGAProjectile::OnCompleted);
	MontageTask->OnBlendOut.AddDynamic(this, &UGAProjectile::OnCompleted);
	MontageTask->OnInterrupted.AddDynamic(this, &UGAProjectile::OnCompleted);
	MontageTask->OnCancelled.AddDynamic(this, &UGAProjectile::OnCompleted);

	FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("char.ability.projectile.triggerEvent"));
	auto EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EventTag);
	EventTask->EventReceived.AddDynamic(this, &UGAProjectile::OnEventRecieved);

	MontageTask->Activate();
	EventTask->Activate();
}

void UGAProjectile::OnCompleted() {
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGAProjectile::OnEventRecieved(FGameplayEventData Payload) {
	auto Owner = Cast<AGASCharacter>(GetOwningActorFromActorInfo());

	do { // "do while 0" pattern
		if (!HasAuthority(&CurrentActivationInfo)) { break; }

		FTransform ProjectileTransform;
		Owner->GetProjectileTransforms_Implementation(ProjectileTransform);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = Owner;
		auto Projectile = GetWorld()->SpawnActor<ABaseProjectile>(Owner->BPProjectileClass, ProjectileTransform, SpawnParams);
		Projectile->EffectSpec = MakeOutgoingGameplayEffectSpec(Owner->GEProjectileDamageClass).Data;
	} while (0);

	//cleanup
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

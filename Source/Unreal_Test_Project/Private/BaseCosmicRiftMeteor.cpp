// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCosmicRiftMeteor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABaseCosmicRiftMeteor::ABaseCosmicRiftMeteor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void ABaseCosmicRiftMeteor::BeginPlay()
{
	Super::BeginPlay();
	if (!EffectSpec) { return; }

	auto SourceCharacter = Cast<IAbilitySystemInterface>(GetInstigator());
	UAbilitySystemComponent* SourceASComp = SourceCharacter->GetAbilitySystemComponent();

	TArray<AActor*> OverlappedActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), 
																						GetActorLocation(),
																						ScanRadius, 
																						{ UEngineTypes::ConvertToObjectType(ECC_Pawn) },
																						nullptr, 
																						{ GetInstigator() },
																						OverlappedActors);

	for (auto Actor : OverlappedActors) {
		auto TargetCharacter = Cast<IAbilitySystemInterface>(Actor);
		if (!TargetCharacter) { break; }

		UAbilitySystemComponent* TargetASComp = TargetCharacter->GetAbilitySystemComponent();
		if (!TargetASComp) { break; }

		SourceASComp->ApplyGameplayEffectSpecToTarget(*EffectSpec, TargetASComp);
	}
	
}

// Called every frame
void ABaseCosmicRiftMeteor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


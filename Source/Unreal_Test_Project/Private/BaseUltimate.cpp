// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseUltimate.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Components/SphereComponent.h"

// Sets default values
ABaseUltimate::ABaseUltimate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	auto SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");

	SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere");
	SphereComponent->SetupAttachment(SceneRoot);
	SphereComponent->SetSphereRadius(350.f);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseUltimate::OnBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseUltimate::OnEndOverlap);

	InitialLifeSpan = 10.0f;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void ABaseUltimate::BeginPlay()
{
	Super::BeginPlay();

}

void ABaseUltimate::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
																	 AActor* OtherActor,
																	 UPrimitiveComponent* OtherComp,
																	 int32 OtherBodyIndex,
																	 bool bFromSweep,
																	 const FHitResult& SweepResult)
{
	if (GetInstigator() == OtherActor) { return; }
	if (!EffectSpec) { return; }

	auto TargetCharacter = Cast<IAbilitySystemInterface>(OtherActor);
	if (!TargetCharacter) { return; }
	
	UAbilitySystemComponent* TargetASComp = TargetCharacter->GetAbilitySystemComponent();
	if (!TargetASComp) { return; }
	
	auto SourceCharacter = Cast<IAbilitySystemInterface>(GetInstigator());
	UAbilitySystemComponent* SourceASComp = SourceCharacter->GetAbilitySystemComponent();
	
	SourceASComp->ApplyGameplayEffectSpecToTarget(*EffectSpec, TargetASComp);
}

void ABaseUltimate::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
																 AActor* OtherActor,
																 UPrimitiveComponent* OtherComp,
																 int32 OtherBodyIndex)
{
	if (GetInstigator() == OtherActor) { return; }

	auto TargetCharacter = Cast<IAbilitySystemInterface>(OtherActor);
	if (!TargetCharacter) { return; }

	UAbilitySystemComponent* TargetASComp = TargetCharacter->GetAbilitySystemComponent();
	if (!TargetASComp) { return; }

	FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("char.ability.ultimate.damageEffect"));
	auto TagContainer = FGameplayTagContainer::CreateFromArray(TArray<FGameplayTag>({ EventTag }));
	TargetASComp->RemoveActiveEffectsWithTags(TagContainer);
}

// Called every frame
void ABaseUltimate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


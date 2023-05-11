#include "BaseProjectile.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"

#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/CollisionProfile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "GASCharacter.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	auto SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	CapsuleComponent->SetupAttachment(SceneRoot);
	CapsuleComponent->InitCapsuleSize(20.0f, 30.0f);
	CapsuleComponent->SetRelativeRotation(FRotator(-90.f,0.f,0.f));
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnBeginOverlap);

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	ParticleSystem->SetupAttachment(CapsuleComponent);
	ParticleSystem->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;

	InitialLifeSpan = 3.0f;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
																		 AActor* OtherActor,
																		 UPrimitiveComponent* OtherComp,
																		 int32 OtherBodyIndex,
																		 bool bFromSweep,
																		 const FHitResult& SweepResult)
{
	if (GetInstigator() == OtherActor) { return; }

	do { // "do while 0" pattern
		auto TargetCharacter = Cast<AGASCharacter>(OtherActor);
		if (!TargetCharacter) { break; }

		UAbilitySystemComponent* TargetASComp = TargetCharacter->GetAbilitySystemComponent();
		if (!TargetASComp) { break; }

		auto SourceCharacter = Cast<AGASCharacter>(GetInstigator());
		UAbilitySystemComponent* SourceASComp = SourceCharacter->GetAbilitySystemComponent();
		
		SourceASComp->ApplyGameplayEffectSpecToTarget(*EffectSpec, TargetASComp);
	} while (0);

	// cleanup
	Destroy();
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


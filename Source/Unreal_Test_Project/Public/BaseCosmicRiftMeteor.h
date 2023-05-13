// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCosmicRiftMeteor.generated.h"

UCLASS()
class UNREAL_TEST_PROJECT_API ABaseCosmicRiftMeteor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseCosmicRiftMeteor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TSharedPtr<struct FGameplayEffectSpec> EffectSpec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScanRadius = 500.f;

};

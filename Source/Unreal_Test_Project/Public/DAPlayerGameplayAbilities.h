// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DAPlayerGameplayAbilities.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_TEST_PROJECT_API UDAPlayerGameplayAbilities : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* PrimaryAttackAMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* PrimaryAttackBMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* PrimaryAttackCMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proyectile")
	TSubclassOf<class UGameplayAbility> GAProyectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proyectile")
	TSubclassOf<class ABaseProjectile> BPProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proyectile")
	TSubclassOf<class UGameplayEffect> GEProjectileDamageClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DarkTether")
	TSubclassOf<class UGameplayAbility> GADarkTetherClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DarkTether")
	TSubclassOf<class UGameplayEffect> GEDarkTetherSlowDownClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CosmicRift")
	TSubclassOf<class UGameplayAbility> GACosmicRiftClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CosmicRift")
	TSubclassOf<class AGameplayAbilityWorldReticle> GARetCosmicRiftClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CosmicRift")
	TSubclassOf<class ABaseCosmicRiftMeteor> BPMeteorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CosmicRift")
	TSubclassOf<class UGameplayEffect> GEMeteorDamageClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TornSpace")
	TSubclassOf<class UGameplayAbility> GATornSpaceClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TornSpace")
	TSubclassOf<class AGameplayAbilityWorldReticle> GARetTornSpaceClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate")
	TSubclassOf<class UGameplayAbility> GAUltimateClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate")
	TSubclassOf<class ABaseUltimate> BPUltimateClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate")
	TSubclassOf<class UGameplayEffect> GEUltimateDamageClass;
	
};

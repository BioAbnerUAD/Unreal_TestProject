// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "InputActionValue.h"

#include "GasCharacterInterface.h"
#include "BasePlayerUI.h"

#include "GASCharacter.generated.h"

class UGASAbilitySystemComponent;
class UGASAttributeSet;

UCLASS()
class UNREAL_TEST_PROJECT_API AGASCharacter : public ACharacter, public IAbilitySystemInterface, public IGasCharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Add Ability System Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GASGameplayAbility")
	class UGASAbilitySystemComponent* AbilitySystemComponent;

	//Add Attribute Set
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GASGameplayAbility")
	const class UGASAttributeSet* AttributeSetVar;

	//Add Variable for Initial Abilities (do not leave blank!)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASGameplayAbility")
	TArray<TSubclassOf<class UGameplayAbility>> InitialAbilities;

	//Interface Function for Ability System Component Getter
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//Native functions for Attribute Change Delagates. Using UFUNCTION() to expose to BP Side Link
	UFUNCTION()
	virtual void OnHealthChangedNative(float Health, int32 StackCount);
	UFUNCTION()
	virtual void OnManaChangedNative(float Mana, int32 StackCount);
	UFUNCTION()
	virtual void OnBaseAttackDamageChangedNative(float BaseAttackDamage, int32 StackCount);
	UFUNCTION()
	virtual void OnSpeedMultiplierChangedNative(float SpeedMultiplier, int32 StackCount);

	//******Event that bind to native events and are implemented in BPs********

	//Event Trigger On Health Change
	UFUNCTION(BlueprintImplementableEvent, Category = "GASGameplayAbility")
	void OnHealthChange(float Health, int32 StackCount);

	//Event Trigger On Mana Change
	UFUNCTION(BlueprintImplementableEvent, Category = "GASGameplayAbility")
	void OnManaChange(float Mana, int32 StackCount);

	//Event Trigger On Base Attack Damage
	UFUNCTION(BlueprintImplementableEvent, Category = "GASGameplayAbility")
	void OnBaseAttackDamageChange(float BaseAttackDamage, int32 StackCount);

	//Event Trigger On Speed Multiplier
	UFUNCTION(BlueprintImplementableEvent, Category = "GASGameplayAbility")
	void OnSpeedMultiplierChange(float SpeedMultiplier, int32 StackCount);

	//Initialize Ability Single
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
	void InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToGet, int32 AbilityLevel);

	//Initialize Ability Multi
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
	void InitializeAbilityMulti(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAcquire, int32 AbilityLevel);

	//Server/Client Functions
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	//*******Ability Values Getter Functions**********

	//Getter for Health Values
	UFUNCTION(BlueprintPure, Category = "GASGameplayAbility")
	void GetHealthValues(float& Health, float& MaxHealth);

	//Getter for Mana Values
	UFUNCTION(BlueprintPure, Category = "GASGameplayAbility")
	void GetManaValues(float& Mana, float& MaxMana);

	//Getter for Base Attack Damage Values
	UFUNCTION(BlueprintPure, Category = "GASGameplayAbility")
	void GetBaseAttackDamageValues(float& BaseAttackDamage);

	//Getter for Speed Multiplier Values
	UFUNCTION(BlueprintPure, Category = "GASGameplayAbility")
	void GetSpeedMultiplierValues(float& SpeedMultiplier);

	//********Ability Modifier Functions********

	//Remove Abilities with Tag
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
	void RemoveAbilityWithTags(FGameplayTagContainer TagContainer);

	//Change Ability Level with Tag
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
	void ChangeAbilityLevelWithTags(FGameplayTagContainer TagContainer, int32 NewLevel);

	//Cancel Ability With Tag
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
	void CancelAbilityWithTags(FGameplayTagContainer WithTags, FGameplayTagContainer WithoutTags);

	//Add Loose Gameplay Tag
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
	void AddLooseGameplayTag(FGameplayTag TagToAdd);

	//Remove Loose Gameplay Tag
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
	void RemoveLooseGameplayTags(FGameplayTag TagsToRemove);

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void PunchAttack();

	void PrimaryAttack();

	void SecondaryAttack();

	void QAbility();

	void EAbility();

	void RUltimate();

public:

	void TriggerSaveAttack_Implementation() override;
	
	void TriggerResetCombo_Implementation() override;

	void PlayAttackAnimation();
	
	void GetHealthValuesUI_Implementation(float& Health, float& MaxHealth) override;
	
	void GetManaValuesUI_Implementation(float& Mana, float& MaxMana) override;

	void GetProjectileTransforms_Implementation(FTransform& SpawnTransforms) override;

	class UDAPlayerGameplayAbilities* GetGameplayAbilityData_Implementation() override;

	FVector CamLineTrace(float TraceRange);

	void SetIsTargeting_Implementation(bool bIsTargeting) override;

	UFUNCTION(Client, Reliable)
	void SetSpeedOnClient(float SpeedMultiplier);

protected:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PrimaryAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SecondaryAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* QAbilityAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* EAbilityAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RUltimateAction;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GASGameplayAbility")
	class UDAPlayerGameplayAbilities* GameplayAbilityData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UBasePlayerUI> PlayerUIClass;

	UBasePlayerUI* PlayerUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combo)
	bool bSaveAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combo)
	bool bIsAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combo)
	int AttackCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsTargeting;

};

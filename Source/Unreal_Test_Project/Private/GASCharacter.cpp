// Fill out your copyright notice in the Description page of Project Settings.


#include "GASCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"

#include "GASAttributeSet.h"
#include "GASAbilitySystemComponent.h"

// Sets default values
AGASCharacter::AGASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);	//Options available are Full, Minimal, Mixed (Mixed is used for easier implementation of Gameplay Cues for replication via Gameplay Effect)
	//For handling Gameplay Cues locally, See GASAbilitySystemComponent.h file's comments

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

}

// Called when the game starts or when spawned
void AGASCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (AbilitySystemComponent)
	{
		//Link Attribute Set to Ability System Component
		AttributeSetVar = AbilitySystemComponent->GetSet<UGASAttributeSet>();

		//Bindings for Attribute Change Delegates
		const_cast<UGASAttributeSet*>(AttributeSetVar)->HealthChangeDelegate.AddDynamic(this, &AGASCharacter::OnHealthChangedNative);
		const_cast<UGASAttributeSet*>(AttributeSetVar)->ManaChangeDelegate.AddDynamic(this, &AGASCharacter::OnManaChangedNative);
		const_cast<UGASAttributeSet*>(AttributeSetVar)->BaseAttackDamageChangeDelegate.AddDynamic(this, &AGASCharacter::OnBaseAttackDamageChangedNative);
		const_cast<UGASAttributeSet*>(AttributeSetVar)->SpeedMultiplierChangeDelegate.AddDynamic(this, &AGASCharacter::OnSpeedMultiplierChangedNative);
	}
	
	PlayerUI = CreateWidget<UBasePlayerUI>(GetWorld(), PlayerUIClass);
	if (PlayerUI != nullptr)
	{
		PlayerUI->AddToViewport();
	}
}

// Called every frame
void AGASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGASCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGASCharacter::Look);

		EnhancedInputComponent->BindAction(PrimaryAttackAction, ETriggerEvent::Started, this, &AGASCharacter::PrimaryAttack);
		EnhancedInputComponent->BindAction(SecondaryAttackAction, ETriggerEvent::Started, this, &AGASCharacter::SecondaryAttack);
		EnhancedInputComponent->BindAction(QAbilityAction, ETriggerEvent::Started, this, &AGASCharacter::QAbility);
		EnhancedInputComponent->BindAction(EAbilityAction, ETriggerEvent::Started, this, &AGASCharacter::EAbility);
		EnhancedInputComponent->BindAction(RUltimateAction, ETriggerEvent::Started, this, &AGASCharacter::RUltimate);

	}
}

void AGASCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGASCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGASCharacter::PunchAttack()
{
	if (bIsAttacking) {
		bSaveAttack = true;
	}
	else {
		bIsAttacking = true;
		PlayAttackAnimation();
	}
}

void AGASCharacter::PrimaryAttack()
{
	AbilitySystemComponent->TryActivateAbilityByClass(GAProyectileClass);
}

void AGASCharacter::SecondaryAttack()
{
}

void AGASCharacter::QAbility()
{
}

void AGASCharacter::EAbility()
{
}

void AGASCharacter::RUltimate()
{
}

void AGASCharacter::TriggerSaveAttack_Implementation()
{
	FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("char.ability.projectile.triggerEvent"));
	FGameplayEventData EventData;
	AbilitySystemComponent->HandleGameplayEvent(EventTag, &EventData);

	//if (bSaveAttack) {
	//	bSaveAttack = false;
	//	PlayAttackAnimation();
	//}
}

void AGASCharacter::TriggerResetCombo_Implementation()
{
	AttackCount = 0;
	bSaveAttack = false;
	bIsAttacking = false;
}

void AGASCharacter::PlayAttackAnimation() {
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) { return; }


	switch (AttackCount) {
	case 0:
		AttackCount = 1;
		AnimInstance->Montage_Play(PrimaryAttackAMontage);
		break;
	case 1:
		AttackCount = 2;
		AnimInstance->Montage_Play(PrimaryAttackBMontage);
		break;
	case 2:
		AttackCount = 0;
		AnimInstance->Montage_Play(PrimaryAttackCMontage);
		break;
	}
}

void AGASCharacter::GetHealthValuesUI_Implementation(float& Health, float& MaxHealth)
{
	GetHealthValues(Health, MaxHealth);
}

void AGASCharacter::GetManaValuesUI_Implementation(float& Mana, float& MaxMana)
{
	GetManaValues(Mana, MaxMana);
}

void AGASCharacter::GetProjectileTransforms_Implementation(FTransform& SpawnTransforms)
{
	FVector TraceLocation = CamLineTrace(2000.f);

	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(FollowCamera->GetComponentLocation(), TraceLocation);

	SpawnTransforms = FTransform(Rotation, GetMesh()->GetSocketLocation("hand_r"), { 1, 1, 1 });
}

FVector AGASCharacter::CamLineTrace(float TraceRange)
{
	FVector WorldLocation = FollowCamera->GetComponentLocation();
	FVector Forward = FollowCamera->GetForwardVector();

	FHitResult OutHit;

	GetWorld()->LineTraceSingleByChannel(OutHit, WorldLocation, Forward * TraceRange + WorldLocation, ECC_Visibility);
	return OutHit.bBlockingHit ? OutHit.Location : OutHit.TraceEnd;
}

UAbilitySystemComponent* AGASCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGASCharacter::OnHealthChangedNative(float Health, int32 StackCount)
{
	OnHealthChange(Health, StackCount);
}

void AGASCharacter::OnManaChangedNative(float Mana, int32 StackCount)
{
	OnManaChange(Mana, StackCount);
}

void AGASCharacter::OnBaseAttackDamageChangedNative(float BaseAttackDamage, int32 StackCount)
{
	OnBaseAttackDamageChange(BaseAttackDamage, StackCount);
}

void AGASCharacter::OnSpeedMultiplierChangedNative(float SpeedMultiplier, int32 StackCount)
{
	OnSpeedMultiplierChange(SpeedMultiplier, StackCount);
}

void AGASCharacter::InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToGet, int32 AbilityLevel)
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToGet, AbilityLevel, 0));
	}
}

void AGASCharacter::InitializeAbilityMulti(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAcquire, int32 AbilityLevel)
{
	for (TSubclassOf<UGameplayAbility> AbilitItem : AbilitiesToAcquire)
	{
		InitializeAbility(AbilitItem, AbilityLevel);
	}
}

void AGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAbilityMulti(InitialAbilities, 0);
}

void AGASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AGASCharacter::GetHealthValues(float& Health, float& MaxHealth)
{
	if (AttributeSetVar)
	{
		Health = AttributeSetVar->GetHealth();
		MaxHealth = AttributeSetVar->GetMaxHealth();
	}
}

void AGASCharacter::GetManaValues(float& Mana, float& MaxMana)
{
	if (AttributeSetVar)
	{
		Mana = AttributeSetVar->GetMana();
		MaxMana = AttributeSetVar->GetMaxMana();
	}
}

void AGASCharacter::GetBaseAttackDamageValues(float& BaseAttackDamage)
{
	if (AttributeSetVar)
	{
		BaseAttackDamage = AttributeSetVar->GetBaseAttackDamage();
	}
}

void AGASCharacter::GetSpeedMultiplierValues(float& SpeedMultiplier)
{
	if (AttributeSetVar)
	{
		SpeedMultiplier = AttributeSetVar->GetSpeedMultiplier();
	}
}

void AGASCharacter::RemoveAbilityWithTags(FGameplayTagContainer TagContainer)
{
	TArray<FGameplayAbilitySpec*> MatchingAbilities;
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, MatchingAbilities, true);
	for (FGameplayAbilitySpec* Spec : MatchingAbilities)
	{
		AbilitySystemComponent->ClearAbility(Spec->Handle);
	}
}

void AGASCharacter::ChangeAbilityLevelWithTags(FGameplayTagContainer TagContainer, int32 NewLevel)
{
	TArray<FGameplayAbilitySpec*> MatchingAbility;
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, MatchingAbility, true);
	for (FGameplayAbilitySpec* Spec : MatchingAbility)
	{
		Spec->Level = NewLevel;
	}
}

void AGASCharacter::CancelAbilityWithTags(FGameplayTagContainer WithTags, FGameplayTagContainer WithoutTags)
{
	AbilitySystemComponent->CancelAbilities(&WithTags, &WithoutTags, nullptr);
}

void AGASCharacter::AddLooseGameplayTag(FGameplayTag TagToAdd)
{
	GetAbilitySystemComponent()->AddLooseGameplayTag(TagToAdd);
	GetAbilitySystemComponent()->SetTagMapCount(TagToAdd, 1);
}

void AGASCharacter::RemoveLooseGameplayTags(FGameplayTag TagsToRemove)
{
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(TagsToRemove);
}


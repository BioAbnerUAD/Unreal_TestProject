// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BasePlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_TEST_PROJECT_API UBasePlayerUI : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Player UI")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category = "Player UI")
	float GetManaPercent() const;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* ManaBar;
};

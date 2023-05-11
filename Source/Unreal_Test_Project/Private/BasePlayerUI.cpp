// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerUI.h"

#include "Components/ProgressBar.h"

#include "GasCharacterInterface.h"

void UBasePlayerUI::NativeConstruct()
{
  if (HealthBar != nullptr)
  {
    HealthBar->PercentDelegate.BindUFunction(this, "GetHealthPercent");
    HealthBar->SynchronizeProperties();
  }
  if (ManaBar != nullptr)
  {
    ManaBar->PercentDelegate.BindUFunction(this, "GetManaPercent");
    ManaBar->SynchronizeProperties();
  }
}

float UBasePlayerUI::GetHealthPercent() const
{
  auto GasCharacter = Cast<IGasCharacterInterface>(GetOwningPlayerPawn());
  if (GasCharacter) {
    float Health, MaxHealth;
    GasCharacter->GetHealthValuesUI_Implementation(Health, MaxHealth);
    return Health / MaxHealth;
  }
  return 0.0f;
}

float UBasePlayerUI::GetManaPercent() const
{
  auto GasCharacter = Cast<IGasCharacterInterface>(GetOwningPlayerPawn());
  if (GasCharacter) {
    float Mana, MaxMana;
    GasCharacter->GetManaValuesUI_Implementation(Mana, MaxMana);
    return Mana / MaxMana;
  }
  return 0.0f;
}

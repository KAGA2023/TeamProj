// Fill out your copyright notice in the Description page of Project Settings.

#include "PerkComponent.h"

UPerkComponent::UPerkComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	m_AllPerks.Empty();
}

void UPerkComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPerkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
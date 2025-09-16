// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

DEFINE_LOG_CATEGORY(LogTriggerComp);

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	UE_LOG(LogTriggerComp, Display, TEXT("Trigger Constructor"));
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTriggerComp, Display, TEXT("Trigger Begined"));
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTriggerComp, Display, TEXT("Trigger Ticked"));
}

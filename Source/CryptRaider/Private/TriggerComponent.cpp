// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"
#include "Mover.h"

DEFINE_LOG_CATEGORY(LogTriggerComp);

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (m_Mover) return;
	
	UActorComponent* ActorRootComp = TargetMover.GetComponent(GetOwner());
	UE_LOG(LogTriggerComp, Warning, TEXT("ActorRootComp: %s"), *ActorRootComp->GetName());
	if (ActorRootComp)
	{
		UE_LOG(LogTriggerComp, Warning, TEXT("MoverComp: %s"), *ActorRootComp->GetOwner()->GetActorNameOrLabel());
		m_Mover = ActorRootComp->GetOwner()->FindComponentByClass<UMover>();
		UE_LOG(LogTriggerComp, Warning, TEXT("m_Mover: %p"), m_Mover);
	}
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* AcceptableActor = GetAcceptableActor();

	if(AcceptableActor)
	{
		UE_LOG(LogTriggerComp, Warning, TEXT("%s: Actor Attached"), *GetOwner()->GetActorNameOrLabel());
		// AcceptableActor의 RootComponent가 UPrimitiveComponent로 캐스팅된다면, 
		// 해당 Actor를 부착시킨다.
		UPrimitiveComponent* AcpPrimiComp = Cast<UPrimitiveComponent>(AcceptableActor->GetRootComponent());
		if (AcpPrimiComp)
		{
			AcpPrimiComp->SetSimulatePhysics(false);

			AcceptableActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

			if (m_Mover)
			{
				UE_LOG(LogTriggerComp, Warning, TEXT("%s: Conected Mover"), *GetOwner()->GetActorNameOrLabel());
				m_Mover->SetShouldMove(true);
			}
		}

		//SetComponentTickEnabled(false);
	}
	else
	{
		if (m_Mover)
		{
			m_Mover->SetShouldMove(false);
		}
	}
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	for (const auto& Actor : Actors)
	{
		if (Actor->ActorHasTag(AcceptableActorTag) && !Actor->ActorHasTag("Grabbed"))
		{
			return Actor;
		}
	}

	return nullptr;
}

void UTriggerComponent::SetMover(UMover* _NewMover)
{
	m_Mover = _NewMover;
}

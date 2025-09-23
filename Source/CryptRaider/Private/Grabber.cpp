// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

DEFINE_LOG_CATEGORY(LogGrabber);

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogGrabber, Warning, TEXT("Check"));

	//피직스핸들컴포넌트 가져오기
	m_PhscsHndlComp =
		GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (m_PhscsHndlComp == nullptr)
	{
		UE_LOG(LogGrabber, Warning, TEXT("No PhysicsHandleComponent!"));
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;

	#pragma region Debug_GrabDistance
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	#pragma endregion

	if (m_PhscsHndlComp && m_PhscsHndlComp->GetGrabbedComponent())
	{
		FVector NewTargetLocation = Start + GetForwardVector() * HoldDistance;
		m_PhscsHndlComp->SetTargetLocationAndRotation(
			NewTargetLocation,
			GetComponentRotation()
		);
	}
}

bool UGrabber::GetGrabbableInReach(FHitResult& _HitResult) const
{
	if (m_PhscsHndlComp == nullptr)
		return false;

	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;

	// 스위프 개시
	return GetWorld()->SweepSingleByChannel(
		_HitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel2, // Grabber 트레이스 채널
		FCollisionShape::MakeSphere(GrabRadius)
	);

}

void UGrabber::Grab()
{
	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);

	if (HasHit)
	{
	#pragma region Debug_Collision
		UE_LOG(LogGrabber, Warning, TEXT("Hit %s"), *HitResult.GetActor()->GetActorNameOrLabel());

		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, false, 20.f);
		DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Green, false, 20.f);
	#pragma endregion

		UPrimitiveComponent* HitComponent = HitResult.GetComponent();

		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();

		HitResult.GetActor()->Tags.Add("Grabbed");

		// 물체잡기
		//HitResult.GetActor()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		m_PhscsHndlComp->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);
	}
	else
	{
		UE_LOG(LogGrabber, Warning, TEXT("NOHit"));
		// 그랩 위치 디버깅
		DrawDebugSphere(GetWorld(), GetComponentLocation() + GetForwardVector() * MaxGrabDistance, 10, 10, FColor::Blue, false, 5.f);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogGrabber, Warning, TEXT("Released"));

	if (m_PhscsHndlComp && m_PhscsHndlComp->GetGrabbedComponent())
	{
		AActor* GrabbedActor = m_PhscsHndlComp->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");

		m_PhscsHndlComp->ReleaseComponent();
	}
}


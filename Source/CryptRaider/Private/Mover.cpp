// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"
#include "Math/UnrealMathUtility.h"

DEFINE_LOG_CATEGORY(LogMover);

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	//��ǥ��ġ ���
	MovedTargetPosition = GetOwner()->GetActorLocation() + MoveOffSet;

	//�����ð����� ��ǥ��ġ������ �ӵ����
	MoveSpeed = FVector::Dist(
		GetOwner()->GetActorLocation(),MovedTargetPosition)
		/ MoveTime;
	
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(ShouldMove)
	{
		//���� ��ġ�� ��ǥ��ġ���� ����
		FVector InterpPosition = FMath::VInterpConstantTo(
			GetOwner()->GetActorLocation(),
			MovedTargetPosition,
			DeltaTime,
			MoveSpeed
		);

		GetOwner()->SetActorLocation(InterpPosition);
	}
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"

class UMover;

#include "TriggerComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTriggerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	AActor* GetAcceptableActor() const;

private:
	UFUNCTION(BlueprintCallable)
	void SetMover(UMover* _NewMover);

private:
	UPROPERTY(EditAnywhere)
	FName AcceptableActorTag;

	UPROPERTY(EditInstanceOnly)
	FComponentReference TargetMover;   // 에디터에서 컴포넌트 선택 UI 제공

private:
	UMover* m_Mover;
	
};

DECLARE_LOG_CATEGORY_EXTERN(LogTriggerComp, Log, All);

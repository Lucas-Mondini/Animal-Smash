// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CombatAnimNotifyState.generated.h"

/**
 * 
 */
class UCombatAnimMontage;
UCLASS()
class ALSV4_CPP_API UCombatAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(VisibleAnywhere)
	UCombatAnimMontage* CombatAnimMontageRef;
	void SetCombatAnimMontageRef(UCombatAnimMontage* NewCombatAnimMontageRef);
};

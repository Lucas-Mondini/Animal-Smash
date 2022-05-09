// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "CombatAnimMontage.generated.h"

/**
 * 
 */
UCLASS()
class ALSV4_CPP_API UCombatAnimMontage : public UObject
{
	GENERATED_BODY()
	
public:
	UCombatAnimMontage();
	
	~UCombatAnimMontage();

	static UCombatAnimMontage* CreateCombatAnimMontage(const TMap<FString, TArray<UCapsuleComponent*>> SocketNameToCollision, AActor* playerRef, const TCHAR* ObjectToFind, float playRate);

	float GetPlayLength();

	void PlayAnimation(float playrate);
	void PlayAnimation();
protected:
	UAnimMontage* AnimMontage;
	class AALSBaseCharacter* ActorToPlayMontage;
	float PlayRate;
	
	
};

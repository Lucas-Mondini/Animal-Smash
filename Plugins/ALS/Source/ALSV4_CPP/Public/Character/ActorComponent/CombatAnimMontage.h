// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"

/**
 * 
 */
class ALSV4_CPP_API CombatAnimMontage
{
public:
	CombatAnimMontage(const TMap<FString, TArray<UCapsuleComponent*>> SocketNameToCollision, AActor* playerRef, const TCHAR* ObjectToFind, float playRate);
	
	~CombatAnimMontage();

	void PlayAnimation();
protected:
	UAnimMontage* AnimMontage;
	class AALSBaseCharacter* ActorToPlayMontage;
	float PlayRate;
	
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatAnimMontage.generated.h"

/**
 * 
 */
class UCapsuleComponent;
UCLASS()
class ALSV4_CPP_API UCombatAnimMontage : public UObject
{
	GENERATED_BODY()
	
public:
	UCombatAnimMontage();
	static UCombatAnimMontage* CreateCombatAnimMontage(const TMap<FString, TArray<FTransform>> SocketNameToCollision, AActor* playerRef, const TCHAR* ObjectToFind, float playRate);
	
	~UCombatAnimMontage();


	void CreateAnimCollisions();
	void DestroyAnimCollisions();
	
	UFUNCTION()
	void CallCombatComponentProcessOverlapProcess(	UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
													UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
													bool bFromSweep, const FHitResult& SweepResult);

	float GetPlayLength();
	void PlayAnimation(float playrate);
	void PlayAnimation();
	
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* AnimMontage;
	UPROPERTY(VisibleAnywhere)
	class AALSBaseCharacter* ActorToPlayMontage;
protected:
	float PlayRate;
	TMap<FString, TArray<FTransform>> SocketNameToCollision;
	TArray<UCapsuleComponent*> CollisionCapsuleArray;
	
};

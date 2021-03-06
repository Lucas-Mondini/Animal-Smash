// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Library/ALSCharacterEnumLibrary.h"
#include "ActorCombatComponent.generated.h"


class UCombatAnimMontage;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSV4_CPP_API UActorCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorCombatComponent();
	
	UPROPERTY()
	TArray<AActor*> ActorHitted;

	UPROPERTY(VisibleAnywhere)
	EACCStateMachine State;

	UPROPERTY(VisibleAnywhere)
	bool CanAttack = true;

	UPROPERTY(EditAnywhere)
	float ComboTime = 1.5f;

	UPROPERTY(VisibleAnywhere)
	float Attack1ComboAccumulator = 0;
	UPROPERTY(VisibleAnywhere)
	float Attack2ComboAccumulator = 0;
	UPROPERTY(VisibleAnywhere)
	float AnimTime = 0;
	UPROPERTY(VisibleAnywhere)
	float AnimAccumulator = 0;
	
	UPROPERTY(EditAnywhere)
	float KickAnimVelocity = 0.8;
	UPROPERTY(EditAnywhere)
	float PunchAnimVelocity = 1;
	UPROPERTY(EditAnywhere)
	float KickAnimOffsetVelocity = 0.65;
	UPROPERTY(EditAnywhere)
	float PunchAnimOffsetVelocity = 0.7;
	
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UCombatAnimMontage*> PunchComboAnimations;
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UCombatAnimMontage*> KickComboAnimations;

	UFUNCTION(BlueprintCallable)
	void Attack1();
	UFUNCTION(BlueprintCallable)
	void Attack2();
	
	FORCEINLINE void setOwnerCharacter(class AALSBaseCharacter* owner) {this->OwnerCharacter = owner;}

	UFUNCTION()
		void CombatComponentProcessOverlapProcess(	UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
														UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
														bool bFromSweep, const FHitResult& SweepResult);
		void HitProcess(AActor* OtherActor);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	unsigned int Attack1ComboCount = 0;

	UPROPERTY(VisibleAnywhere)
	unsigned int Attack1ComboLength;

	UPROPERTY(VisibleAnywhere)
	unsigned int Attack2ComboCount = 0;
	
	UPROPERTY(VisibleAnywhere)
	unsigned int Attack2ComboLength;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(VisibleAnywhere)
	class AALSBaseCharacter* OwnerCharacter;
		
};

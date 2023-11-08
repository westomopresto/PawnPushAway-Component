// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Math/UnrealMathUtility.h"
#include "AC_PawnPushAway.generated.h"


/** Please add a class description */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PRAXIS_API UAC_PawnPushAway : public UActorComponent
{
	GENERATED_BODY()

public:
	UAC_PawnPushAway();
	/** Please add a variable description */
	UPROPERTY(Transient)
	TArray<ACharacter*> OverlappedPawns;


	/** Please add a variable description */
	UPROPERTY(Transient)
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float TickRate = 0.016667f; // Default tick rate (adjust as needed)

	UPROPERTY(EditAnywhere, Category = "Settings")
	float InterpolationSpeed = 8.0f; // Default interpolation speed (adjust as needed)

	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

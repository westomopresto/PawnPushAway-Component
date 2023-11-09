// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/ShapeComponent.h"
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
	TObjectPtr<UShapeComponent> ShapeComponent;

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool DrawDebug = false;

	UPROPERTY(EditAnywhere, Category = "Settings")
	FName PushShapeTag = "PushShape"; // tag of the push shape we're gonna use

	UPROPERTY(EditAnywhere, Category = "Settings")
	float TickRate = 0.016667f; // Default tick rate (adjust as needed)

	UPROPERTY(EditAnywhere, Category = "Settings")
	float InterpolationSpeed = 2.5f; // Default interpolation speed (adjust as needed)

	UFUNCTION()
	void OnShapeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnShapeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FColor DebugColor;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

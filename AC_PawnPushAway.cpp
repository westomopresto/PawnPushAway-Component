// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_PawnPushAway.h"

// Sets default values for this component's properties
UAC_PawnPushAway::UAC_PawnPushAway()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UAC_PawnPushAway::BeginPlay()
{
	Super::BeginPlay();
    PrimaryComponentTick.TickInterval = TickRate;
    AActor* OwnerActor = GetOwner();

    // Attempt to cast the owner Actor to ACharacter
    ACharacter* CharacterOwner = Cast<ACharacter>(OwnerActor);

    if (CharacterOwner)
    {
        // If the cast is successful, get the CapsuleComponent of the ACharacter
        UCapsuleComponent* CharacterCapsule = CharacterOwner->FindComponentByClass<UCapsuleComponent>();

        if (CharacterCapsule)
        {
            // Store the reference to the CapsuleComponent in your component's CapsuleComponent property
            CapsuleComponent = CharacterCapsule;
            CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &UAC_PawnPushAway::OnCapsuleBeginOverlap);
            CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &UAC_PawnPushAway::OnCapsuleEndOverlap);
        }
        else
        {
            // Handle the case where the CapsuleComponent is not found
            UE_LOG(LogTemp, Warning, TEXT("CapsuleComponent not found on ACharacter owner."));
        }
    }
    else
    {
        // Handle the case where the owner is not an ACharacter
        UE_LOG(LogTemp, Warning, TEXT("Owner is not an ACharacter."));
    }
}

void UAC_PawnPushAway::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
    if (OtherCharacter)
    {
        // Add the overlapping ACharacter to the OverlappedPawns array
        OverlappedPawns.Add(OtherCharacter);
        SetComponentTickEnabled(true);
    }
}

void UAC_PawnPushAway::OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
    if (OtherCharacter)
    {
        // Remove the overlapping ACharacter from the OverlappedPawns array
        OverlappedPawns.Remove(OtherCharacter);
        if (OverlappedPawns.Num() == 0)
        {
            SetComponentTickEnabled(false);
        }
    }
}

// Called every frame
void UAC_PawnPushAway::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (OverlappedPawns.Num() < 1)
    {
        // If there are no overlapping ACharacters, disable the component's tick
        SetComponentTickEnabled(false);
        return;
    }

    // Get the owning actor's location
    FVector OwnerLocation = GetOwner()->GetActorLocation();

    for (ACharacter* OverlappingCharacter : OverlappedPawns)
    {
        // Get the ACharacter's location
        FVector CharacterLocation = OverlappingCharacter->GetActorLocation();
        FRotator CharacterRotation = OverlappingCharacter->GetActorRotation();

        // Calculate the direction vector
        FVector Direction = CharacterLocation - OwnerLocation;

        // Normalize the direction vector in 2D (ignoring Z)
        Direction = Direction.GetSafeNormal2D();

        // Multiply the normalized vector by the owner's capsule component radius
        FVector PushLocation = OwnerLocation + Direction * CapsuleComponent->GetUnscaledCapsuleHalfHeight();

        // Use VInterp to smoothly move the ACharacter to the new location
        FVector NewLocation = FMath::VInterpTo(CharacterLocation, PushLocation, DeltaTime, InterpolationSpeed); 

        // Teleport the ACharacter to the new location
        OverlappingCharacter->TeleportTo(NewLocation, CharacterRotation);
    }
}


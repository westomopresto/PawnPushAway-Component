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
        UShapeComponent* PushShape = CharacterOwner->FindComponentByTag<UShapeComponent>(PushShapeTag);

        if (PushShape)
        {
            // Store the reference to the CapsuleComponent in your component's CapsuleComponent property
            ShapeComponent = PushShape;
            ShapeComponent->OnComponentBeginOverlap.AddDynamic(this, &UAC_PawnPushAway::OnShapeBeginOverlap);
            ShapeComponent->OnComponentEndOverlap.AddDynamic(this, &UAC_PawnPushAway::OnShapeEndOverlap);
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

    uint8 Red = FMath::RandRange(0, 255);
    uint8 Green = FMath::RandRange(0, 255);
    uint8 Blue = FMath::RandRange(0, 255);
    uint8 Alpha = 255;  // You can adjust the alpha value as needed

    // Create a random FColor
    DebugColor = FColor(Red, Green, Blue, Alpha);
}

void UAC_PawnPushAway::OnShapeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
    if (OtherCharacter)
    {
        // Add the overlapping ACharacter to the OverlappedPawns array
        OverlappedPawns.Add(OtherCharacter);
        SetComponentTickEnabled(true);
    }
}

void UAC_PawnPushAway::OnShapeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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


        // Get the bounds of the shape component
        float Radius = ShapeComponent->Bounds.SphereRadius;
        float RadiusB = OverlappingCharacter->FindComponentByTag<UShapeComponent>(PushShapeTag)->Bounds.SphereRadius;
        float CombinedRadius = Radius + RadiusB + 5.0f;

        // Scale push strength by distance
        float Distance = FVector::Dist(OwnerLocation, CharacterLocation);
        float PushStrength = 1-(FMath::Clamp(((Distance / CombinedRadius)),0.0f, 1.0f));


        // Multiply the normalized vector by the owner's capsule component radius
        FVector PushLocation = OwnerLocation + Direction * CombinedRadius;
        PushLocation.Z = CharacterLocation.Z; // Sets Z to character Z, only pushing on Z test

        // Use VInterp to smoothly move the ACharacter to the new location
        FVector NewLocation = FMath::VInterpTo(CharacterLocation, PushLocation, DeltaTime, InterpolationSpeed * PushStrength);

        // Teleport the ACharacter to the new location
        OverlappingCharacter->TeleportTo(NewLocation, CharacterRotation);

        if (DrawDebug) {
            //debug
            // Debug line for the direction vector
            DrawDebugLine(GetWorld(), OwnerLocation, OwnerLocation + Direction * CombinedRadius, DebugColor, false, -1, 0, 1);

            // Debug box for new location
            FVector DebugBoxExtents = FVector(15.0f, 15.0f, 15.0f); // Set the box size
            FVector DebugBoxCenter = PushLocation;
            DrawDebugBox(GetWorld(), DebugBoxCenter, DebugBoxExtents, DebugColor, false, -1, 0, 1);

            DrawDebugBox(GetWorld(), OwnerLocation, DebugBoxExtents/2, DebugColor, false, -1, 0, 1);
            UE_LOG(LogTemp, Warning, TEXT("PushStrength: %f"), PushStrength);
        }
        
    }
}


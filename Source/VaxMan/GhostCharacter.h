// Author: Christian Pazmino 
//for Forage - Electronic Arts, the third challenge of the program
// i'm really enjoying it!

#pragma once
#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "GhostCharacter.generated.h"


class USphereComponent;
class USoundCue;
class UPaperFlipbookComponent;
class UPaperFlipbook;
UCLASS()
class VAXMAN_API AGhostCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	
	AGhostCharacter();
	FVector CurrentLocation;
	UPROPERTY(EditAnywhere)
		float  delayToStart;
	UPROPERTY(EditAnywhere)
		float  StartVertical = 0.f;
	UPROPERTY(EditAnywhere)
		float  StartHorizontal= 0.f;
	bool directions[4] = { false,false,false,false };
	bool bStarted = false;
	USoundCue* eating;
	class USoundBase* eatSound;
	UPROPERTY(EditAnywhere)
		FVector2D dir = FVector2D(0.f,0.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString name;
	UPROPERTY(EditAnywhere)
		USphereComponent* GhostSphere;
	UPROPERTY(EditAnywhere)
		UPaperFlipbookComponent* Flipbook;
	UPaperFlipbook* Blinky;
	UPaperFlipbook* Inky;
	UPaperFlipbook* Pinky;
	UPaperFlipbook* Clyde;
	class APacman* pc;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OverlapPacman(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void MoveForward(float Value);
	UFUNCTION(BlueprintCallable)
		void CheckDirection();
	UFUNCTION(BlueprintCallable)
		void GetOut();
	// Handles input for moving right and left.
	UFUNCTION()
		void MoveRight(float Value);
	UFUNCTION()
		void initialize(FString GhostName, float GhostStartHorizontal, float GhostStartVertical, float GhostDelay);


	void StartToMove();
	void SpawnDefaultController();
};

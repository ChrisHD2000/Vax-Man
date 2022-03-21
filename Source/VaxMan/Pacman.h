// Author: Christian Pazmino 
//for Forage - Electronic Arts, the third challenge of the program
// i'm really enjoying it!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Pacman.generated.h"


class UCameraComponent;
class USphereComponent;


UCLASS()
class VAXMAN_API APacman : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerController* MyController;
	APacman();
	UPROPERTY(BlueprintReadWrite)
		int Score = 0;	
	UPROPERTY(BlueprintReadWrite)
		bool bWin = false;
	int mov = 0;
	int isForward = 0.f;
	int isRight = 0.f;

	bool justAte = true; 
	UPROPERTY(EditAnywhere)
		UCameraComponent* cam;
	UPROPERTY(EditAnywhere)
		USphereComponent* Sphere;
	UPROPERTY()
		class UPacmanMovementComponent* PacmanMovementComponent;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual UPawnMovementComponent* GetMovementComponent() const override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InInputComponent) override;
	UFUNCTION()
	void UpdateScore();
private:
	void MoveForward();
	void MoveRight();
	void MoveBack();
	void MoveLeft();
};

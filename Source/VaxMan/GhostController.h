// Author: Christian Pazmino 
//for Forage - Electronic Arts, the third challenge of the program
// i'm really enjoying it!

#pragma once
#include "GhostCharacter.h"
#include  "Pacman.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "GhostController.generated.h"

/**
 * 
 */
UCLASS()
class VAXMAN_API AGhostController : public AAIController
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

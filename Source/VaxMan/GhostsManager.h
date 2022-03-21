// Author: Christian Pazmino 
//for Forage - Electronic Arts, the third challenge of the program
// i'm really enjoying it!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GhostsManager.generated.h"
class USoundCue;
UCLASS()
class VAXMAN_API AGhostsManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGhostsManager();
	bool bStopDuplicating = true;
	UPROPERTY(BlueprintReadOnly)
		int numGhosts;
	FTimerHandle delay;
	USoundCue* booWomp;
	UPROPERTY(EditDefaultsOnly, Category = Instrumento)
		TSubclassOf<class AGhostCharacter> GhostClass;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ShouldDuplicate();
	void DuplicateGhosts(FVector2D dir);
	UFUNCTION(BlueprintCallable)
		void StopTimer();
};

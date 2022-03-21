// Author: Christian Pazmino 
//for Forage - Electronic Arts, the third challenge of the program
// i'm really enjoying it!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Food.generated.h"
class UStaticMeshComponent;
class USphereComponent;
class USoundCue;
UCLASS()
class VAXMAN_API AFood : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* foodMesh;
	UPROPERTY(EditAnywhere)
	USphereComponent* foodSphere;
	USoundCue* eating;
	AFood();
	class USoundBase* eatSound;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OverlapPacman(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};

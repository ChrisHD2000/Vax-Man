// Author: Christian Pazmino 
//for Forage - Electronic Arts, the third challenge of the program
// i'm really enjoying it!


#include "GhostsManager.h"
#include "TimerManager.h"
#include "GhostCharacter.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

// this class handles the ghosts multiplication //

AGhostsManager::AGhostsManager()
{
 	PrimaryActorTick.bCanEverTick = true;
	//loads the sound fx for the ghost multiplication
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundMultiplying(TEXT("/Game/Sounds/Boowamp_Cue"));
	if (SoundMultiplying.Succeeded()) {
		booWomp = SoundMultiplying.Object;
	}
}

void AGhostsManager::BeginPlay()
{
	Super::BeginPlay();
	//waits 30 seconds for the ShouldDuplicate function
	GetWorldTimerManager().SetTimer(delay, this, &AGhostsManager::ShouldDuplicate, 30.f);
}

// Called every frame
void AGhostsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//We need to expose the number of ghosts numGhosts in the game for the HUD info
	TArray<AActor*> ActorsFound;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), GhostClass, ActorsFound);
	numGhosts = ActorsFound.Num();
	// calls the function DuplicateGhosts every frame
	DuplicateGhosts(FVector2D(1.f, 0.f));
}

void AGhostsManager::DuplicateGhosts(FVector2D dir)
{
	//duplicates the ghosts only when the variable bStopDuplicating is false 
	if (!bStopDuplicating) {			
		TArray<AActor*> ActorsFound;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), GhostClass, ActorsFound);		
		if(ActorsFound.Num()>0)
			UGameplayStatics::PlaySound2D(this, booWomp); // plays the multiplication sound 
		for (int i = 0; i < ActorsFound.Num(); i++)	//for each ghost in the game, another one with the same characteristics will spawn
		{
			FTransform SpawnTransform(ActorsFound[i]->GetTransform());
			auto NewGhost = Cast<AGhostCharacter>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, GhostClass, SpawnTransform));
			if (NewGhost) {
				auto Ghost = Cast<AGhostCharacter>(ActorsFound[i]);
				NewGhost->SpawnDefaultController();
				NewGhost->initialize(Ghost->name, Ghost->dir.Y, Ghost->dir.X, 1.f);
				UGameplayStatics::FinishSpawningActor(NewGhost, SpawnTransform);
			}
			GetWorldTimerManager().SetTimer(delay, this, &AGhostsManager::ShouldDuplicate, 30.f); // calls to duplicate the next 30 secs
				bStopDuplicating = true; // avoids to duplicate the ghosts in every grame
		}
	}
}

void AGhostsManager::ShouldDuplicate()
{
	bStopDuplicating = false;
}
void AGhostsManager::StopTimer()
{
	GetWorldTimerManager().ClearTimer(delay);
}


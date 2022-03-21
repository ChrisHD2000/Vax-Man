// Author: Christian Pazmino 
//for Forage - Electronic Arts, the third challenge of the program
// i'm really enjoying it!


#include "Food.h"
#include "GameFramework/PlayerController.h"
#include "Components/SphereComponent.h"
#include "Pacman.h"
#include "UObject/ConstructorHelpers.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "GhostCharacter.h"
// Sets default values

AFood::AFood()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	foodMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = foodMesh;

	// creates the overlaping shpere that detects collision with pacman
	foodSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollydeSphere"));
	foodSphere->SetupAttachment(RootComponent);
	foodSphere->SetSphereRadius(15.f);
	foodSphere->OnComponentBeginOverlap.AddDynamic(this, &AFood::OverlapPacman);
	foodSphere->SetGenerateOverlapEvents(true);
	
	//eating sound effect
	static ConstructorHelpers::FObjectFinder<USoundCue> waka(TEXT("/Game/Sounds/Waka_Cue"));
	if (waka.Succeeded()) {
		eatSound = waka.Object;

	}

}

// Called when the game starts or when spawned
void AFood::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	

	
}

//method  called when pacman overlaps the food
void AFood::OverlapPacman(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APacman* pc = Cast<APacman>(OtherActor);
	if (pc) {
		pc->UpdateScore(); // updates the score of pacman
		if (!pc->justAte) {  // the sound will play alternately
			UGameplayStatics::PlaySound2D(this, eatSound);
			pc->justAte = true;
		}
		else {
			pc->justAte = false;
		}
		//UE_LOG(LogTemp, Warning, TEXT("Score: %i"), pc->Score)
		Destroy();
	}
	AGhostCharacter* gc = Cast<AGhostCharacter>(OtherActor);
	if (gc){
			
	}
}


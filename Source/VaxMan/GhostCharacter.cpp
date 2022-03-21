// Author: Christian Pazmino 
//for Forage - Electronic Arts, the third challenge of the program
// i'm really enjoying it!


#include "GhostCharacter.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Pacman.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "GhostController.h"
AGhostCharacter::AGhostCharacter() {
	// creation of the flipbook component for the visual ghost
	Flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook"));
	Flipbook->SetupAttachment(RootComponent);
	Flipbook->SetWorldRotation(FRotator(-90.f, 90.f, 0.f));
	Flipbook->SetWorldScale3D(FVector(.35f, 0.35f, .35f));

	//loads every flipbook of each ghost
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> BlinkyFlipbook(TEXT("/Game/Sprites/Ghosts/BlinkyP.BlinkyP"));
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> InkyFlipbook(TEXT("/Game/Sprites/Ghosts/InkyP.InkyP"));
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> PinkyFlipbook(TEXT("/Game/Sprites/Ghosts/PinkyP.PinkyP"));
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> ClydeFlipbook(TEXT("/Game/Sprites/Ghosts/ClydeP.ClydeP"));
	
	//assigns the loaded flipbook to the correspondent ghost
	if (BlinkyFlipbook.Succeeded()) { Blinky = BlinkyFlipbook.Object;}
	if (InkyFlipbook.Succeeded()) {Inky = InkyFlipbook.Object;}
	if (PinkyFlipbook.Succeeded()) {Pinky = PinkyFlipbook.Object;}
	if (ClydeFlipbook.Succeeded()) {Clyde = ClydeFlipbook.Object;}

	//creates the overlaping sphere of the ghosts
	GhostSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	GhostSphere->BodyInstance.SetCollisionProfileName(TEXT("Ghost"));
	GhostSphere->SetupAttachment(RootComponent);
	GhostSphere->SetSphereRadius(50.f);	
	GhostSphere->SetGenerateOverlapEvents(true);		
	
	//loads the waka waka sound effect
	static ConstructorHelpers::FObjectFinder<USoundCue> waka(TEXT("/Game/Sounds/Bite_Cue"));
	if (waka.Succeeded()) {
		eatSound = waka.Object;
	}

	
}
void AGhostCharacter:: BeginPlay() {
	Super::BeginPlay();
	dir = FVector2D(StartVertical, StartHorizontal);// sets initial direction for the ghosts (each has a different one)
	UE_LOG(LogTemp, Warning, TEXT("%f, %f"), dir.X, dir.Y)
		//sets the correspondent flipbook to the flipbookcomponent for visual representation depending on the ghost name
	if (name == "Blinky") { Flipbook->SetFlipbook(Blinky);} 
	if (name == "Inky") { Flipbook->SetFlipbook(Inky); }
	if (name == "Pinky") { Flipbook->SetFlipbook(Pinky); }
	if (name == "Clyde") { Flipbook->SetFlipbook(Clyde); }
	
	//binds the OverlapPacman function when the overlaping sphere overlaps something
	GhostSphere->OnComponentBeginOverlap.AddDynamic(this, &AGhostCharacter::OverlapPacman);
	
	//gives a specific delay for each ghost to start moving on the game
	FTimerHandle timeToStart;	
	GetWorldTimerManager().SetTimer(timeToStart, this, &AGhostCharacter::StartToMove, delayToStart);
}

void AGhostCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bStarted) { //when this variable is true, the ghosts start to move in their direction
		MoveRight(dir.Y);
		MoveForward(dir.X);
	}
	//handles the position of the ghosts when they pass by the side gates
	if (GetActorLocation().Y > 1150.f)
		SetActorLocation(FVector(GetActorLocation().X, -1150.f, GetActorLocation().Z));
	if (GetActorLocation().Y < -1150.f)
		SetActorLocation(FVector(GetActorLocation().X, 1150.f, GetActorLocation().Z));
}

//method bound with the overlaping sphere, destroys the ghost whenever they overlap pacman
void AGhostCharacter::OverlapPacman(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	pc = Cast<APacman>(OtherActor);
	if (pc) {		
		UGameplayStatics::PlaySound2D(this, eatSound); //plays the eat sound vefore destroying
		Destroy();
	}
	AGhostCharacter* gc = Cast<AGhostCharacter>(OtherActor);
	if (gc) {}
}

void AGhostCharacter::MoveRight(float Value) {
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value*20.f);	
}

void AGhostCharacter::MoveForward(float Value) {
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value * 20.f);
}

// checks the direciton of the ghosts depending of the intersection they are located
void AGhostCharacter::CheckDirection() {
	//we don't want the ghosts to detect pacman and themselves to define their direction, but the walls around 
	TArray <AActor*> actorsIgnored;
	actorsIgnored.Add(pc);
	actorsIgnored.Add(this);

	//we need to detect if there's a wall in front, back, right and left of each ghost 
	FHitResult resultFront;
	FCollisionObjectQueryParams  ParmsFront;
	ParmsFront.ObjectTypesToQuery = FCollisionObjectQueryParams::AllStaticObjects; 

	FHitResult resultRight;
	FCollisionObjectQueryParams ParmsRight;
	ParmsRight.ObjectTypesToQuery = FCollisionObjectQueryParams::AllStaticObjects;

	FHitResult resultBack;
	FCollisionObjectQueryParams ParmsBack;
	ParmsBack.ObjectTypesToQuery = FCollisionObjectQueryParams::AllStaticObjects;

	FHitResult resultLeft;
	FCollisionObjectQueryParams ParmsLeft;
	ParmsLeft.ObjectTypesToQuery = FCollisionObjectQueryParams::AllStaticObjects;

	// the array "directions" stores in what direction of the ghost was found a wall 
	for (int i = 0; i < 4; i++) {
		directions[i] = true;
	}
	// if there's no wall detected its false, else it's true
	GetWorld()->LineTraceSingleByObjectType(resultFront, FVector(GetActorLocation()),
		FVector(GetActorLocation() + GetActorForwardVector() * 100.f), ParmsFront);
	if (!resultFront.GetActor()) {		
		directions[1] = false;
	}
	else {
		directions[1] = true;				
	}
	GetWorld()->LineTraceSingleByObjectType(resultRight, FVector(GetActorLocation()),
		FVector(GetActorLocation() + GetActorRightVector() * 100.f), ParmsRight);
	if (!resultRight.GetActor()) {		
		directions[2] = false;
	}
	else {
		directions[2] = true;		
	}
	GetWorld()->LineTraceSingleByObjectType(resultBack, FVector(GetActorLocation()),
		FVector(GetActorLocation() + GetActorForwardVector() * -100.f), ParmsBack);
	if (!resultBack.GetActor()) {		
		directions[3] = false;		
	}
	else {
		directions[3] = true;	
	}
	GetWorld()->LineTraceSingleByObjectType(resultLeft, FVector(GetActorLocation()),
		FVector(GetActorLocation() + GetActorRightVector() * -100.f), ParmsLeft);
	if (!resultLeft.GetActor()) {
		directions[0] = false;
	}
	else {
		directions[0] = true;			
	}

	// there are 9 different kinds of intersections, each one has at least 2 different directions available
	//the variable rand decides randomly what direction the ghost will take for each situation
	int32 min = 1;
	int32 max = 3;
	int32 rand;
	rand = (int32) FMath::RandRange(min, max);	
	if (!directions[0] && !directions[1] && !directions[3] && directions[2]) 
	{ 	
		if (rand == 1)	{
			dir = FVector2D(1.f, 0.f);	return;
		}
		if (rand == 2)
		{
			dir = FVector2D(0.f, -1.0f); return;
		}
		if (rand == 3)	
		{
			dir = FVector2D(-1.f, 0.0f);  return;
		}
	}
	if (!directions[0] && !directions[2] && !directions[3] && directions[1]) {
		if (rand == 1) {
			dir = FVector2D(0.f, -1.f); return;
		}
		if (rand == 2) {
			dir = FVector2D(0.f, 1.f); return;
		}
		if (rand == 3) {
			dir = FVector2D(-1.f, 0.0f); return;
		}		
	}
	if (!directions[0] && !directions[1] && !directions[2] && directions[3]) { 
		if (rand == 1) {			
			dir = FVector2D(1.f,0.f); return;
		}
		if (rand == 2) {			
			dir = FVector2D(0.f, 1.f); return;
		}
		if (rand == 3) {
			
			dir = FVector2D(0.f, -1.f); return;
		}
	}
	if (!directions[1] && !directions[2] && !directions[3] && directions[0]) {
		if (rand == 1) {			
			dir = FVector2D(-1.f, 0.f); return;
		}
		if (rand == 2) {
			dir = FVector2D(1.f, 0.f); return;
		}
		if (rand == 3) {
			dir = FVector2D(0.f, 1.f); return;
		}
	}

	if (!directions[0] && !directions[1]  && !directions[2] && !directions[3]) {
		if (rand == 1) {
			dir = FVector2D(-1.f, 0.f); return;
		}
		if (rand == 2) {
			dir = FVector2D(1.f, 0.f);  return;
		}
		if (rand == 3){
			dir = FVector2D(0.f, -1.f); return;
		}
	}

	if (!directions[1] && !directions[2] && directions[0] && directions[3]) { 
		if (rand == 1) {
			dir = FVector2D(1.f, 0.f); return;
		}
		if (rand == 2) {
			dir = FVector2D(0.f, 1.f);  return;
		}
		if (rand == 3) {
			dir = FVector2D(0.f, 1.f); return;
		}		
	}
	if (!directions[3] && !directions[2] && directions[1] && directions[0]) { 
		if (rand == 1) {
			dir = FVector2D(0.f, 1.f); return;
		}
		if (rand == 2) {
			dir = FVector2D(-1.f, 0.f);  return;
		}
		if (rand == 3) {
			dir = FVector2D(-1.f, 0.f); return;
		}		
	}
	if (!directions[0] && !directions[1] && directions[2] && directions[3]) {
		if (rand == 1) {
			dir = FVector2D(1.f, 0.f); return;
		}
		if (rand == 2) {
			dir = FVector2D(0.f, -1.f);  return;
		}
		if (rand == 3) {
			dir = FVector2D(0.f, -1.f); return;
		}

	}
	if (!directions[0] && !directions[3] && directions[1] && directions[2]) {
		if (rand == 1) {
			dir = FVector2D(-1.f, 0.f); return;
		}
		if (rand == 2) {
			dir = FVector2D(0.f, -1.f);  return;
		}
		if (rand == 3) {
			dir = FVector2D(0.f, -1.f); return;
		}

	}
}

// if the ghosts enter the cage, this method will be called
void AGhostCharacter::GetOut() {
	dir = FVector2D(1.f, 0.0f);  return;
}
// changes to true when the time to start has finished
void AGhostCharacter::StartToMove() {

	bStarted = true;
}

//these are the params which every new duplicated ghost will spawn in the world
void AGhostCharacter::initialize(FString GhostName, float GhostStartHorizontal, float GhostStartVertical, float GhostDelay) {
	this->name = GhostName;
	this->StartHorizontal = GhostStartHorizontal;
	this->StartVertical = GhostStartVertical;
	this->delayToStart = GhostDelay;
}

//Spawns a new AI controller for each duplicated ghost
void AGhostCharacter::SpawnDefaultController() {
	if (Controller != NULL || GetNetMode() == NM_Client)
	{
		return;
	}
	if (AIControllerClass != NULL)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = this;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.OverrideLevel = GetLevel();
		SpawnInfo.ObjectFlags |= RF_Transient;    // We never want to save AI controllers into a map
		AController* NewController = GetWorld()->SpawnActor<AController>(AIControllerClass, GetActorLocation(), GetActorRotation(), SpawnInfo);
		if (NewController != NULL)
		{
			// if successful will result in setting this->Controller 
			// as part of possession mechanics
			NewController->Possess(this);
		}
	}
}

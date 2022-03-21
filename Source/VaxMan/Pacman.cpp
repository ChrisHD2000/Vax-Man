// Author: Christian Pazmino 
//for Forage - Electronic Arts, the third challenge of the program
// i'm really enjoying it!


#include "Pacman.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/SphereComponent.h"
#include "PacmanMovementComponent.h"

// Sets default values
APacman::APacman()
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
 	// Creates the collision sphere of pacman
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollide"));
	RootComponent = Sphere;
	Sphere->SetSphereRadius(48.f);;
	Sphere->SetCollisionProfileName(TEXT("Pawn"));
	
	//sets the camera right on top, ortographic, facing the board
	cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cam->SetRelativeLocation(FVector(0.f, 0.f, 1000.f));
	cam->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	cam->SetProjectionMode(ECameraProjectionMode::Orthographic);
	cam->OrthoWidth = 2100.f;
	cam->OrthoFarClipPlane = 1500.f;
	cam->SetupAttachment(RootComponent);

	//creates pacman movement component
	PacmanMovementComponent = CreateDefaultSubobject<UPacmanMovementComponent>(TEXT("Pacman Movement"));
	PacmanMovementComponent->UpdatedComponent = RootComponent;
}

// Called when the game starts or when spawned
void APacman::BeginPlay()
{
	Super::BeginPlay();
	//the camera should not follow pacman
	cam->DetachFromParent(true, false);

	Sphere->SetRelativeLocation(GetActorLocation()+ FVector(-200.f, 0.f, 0.f));

	
}

// Called every frame
void APacman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//handles the position of pacman when he passes by the side gates
	if (GetActorLocation().Y > 1100.f)
		SetActorLocation(FVector(GetActorLocation().X, -1100.f, GetActorLocation().Z));
	if (GetActorLocation().Y < -1100.f)
		SetActorLocation(FVector(GetActorLocation().X, 1100.f, GetActorLocation().Z));

	//pending on the value of isForward and isRight, pacman will move to a certain direction each frame
	if (isForward!=0) {
		if (PacmanMovementComponent && (PacmanMovementComponent->UpdatedComponent == RootComponent))
		{
			PacmanMovementComponent->AddInputVector(GetActorForwardVector() * isForward);
		}
	}
	if (isRight!=0) {
		if (PacmanMovementComponent && (PacmanMovementComponent->UpdatedComponent == RootComponent))
		{
			PacmanMovementComponent->AddInputVector(GetActorRightVector() * isRight);
		}
	}
	
}

// Called to bind functionality to input
void APacman::SetupPlayerInputComponent(class UInputComponent* InInputComponent)
{
	Super::SetupPlayerInputComponent(InInputComponent);
	InInputComponent->BindAction("MoveForward", IE_Pressed, this, &APacman::MoveForward);
	InInputComponent->BindAction("MoveBack", IE_Pressed, this, &APacman::MoveBack);
	InInputComponent->BindAction("MoveLeft", IE_Pressed, this, &APacman::MoveLeft);
	InInputComponent->BindAction("MoveRight", IE_Pressed, this, &APacman::MoveRight);

}

//method called when pacman eates a ghost
void APacman::UpdateScore()
{
	Score += 10;
	if (Score >= 1980) {
		bWin = true; //variable called to blueprints when winning the game
	}
}

//sets value to isForward and isRight
void APacman::MoveForward() 
{
	isForward = 1;
	isRight = 0;
}

void APacman::MoveRight()
{
	isForward = 0;
	isRight = 1;
}

void APacman::MoveLeft()
{
	isForward = 0;
	isRight = -1;
}

void APacman::MoveBack()
{
	isForward = -1;
	isRight = 0;
}

UPawnMovementComponent* APacman::GetMovementComponent() const
{
	return PacmanMovementComponent;
}

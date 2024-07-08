// Copyright PandoraCube


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	/*
	 * Line trace from Cursor.
	 * 1. Last is null && This is null
	 *		- Do nothing.
	 * 2. Last is null && This is Vaild
	 *		- Highlight This
	 * 3. Last is Valid && This is null
	 *		- UnHighlight Last
	 * 4. Both are Valid, But Last != This
	 *		- UnHightlight Last && Highlight This
	 * 5. Both are Valid && Last == This
	 *		- Do nothing.
	 */
	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// Case 2.
			ThisActor->HighlightActor();
		}
		else
		{
			// Case 1.
			// Do nothing.
		}
	}
	else
	{
		if (ThisActor == nullptr)
		{
			// Case 3.
			LastActor->UnHighlightActor();
		}
		else 
		{
			if (ThisActor != LastActor)
			{
				// Case 4.
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				// Case 5.
				// Do nothing.
			}
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);		// Check AuraContext and when it doesn't, get CRASH

	// Get Input Subsystem for Local player
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	// Cursor Setting
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);		// Cast with Check()

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();	// Get Input Action to X,Y Axis
	const FRotator Rotation = GetControlRotation();		// Get Controller's Rotation
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);		// Extract Yaw (Pitch, Roll = 0)

	// Extract Unit Vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Add Input to Pawn
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}


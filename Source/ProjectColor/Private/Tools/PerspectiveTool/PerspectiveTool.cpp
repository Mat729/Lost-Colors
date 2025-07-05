#include "Tools/PerspectiveTool/PerspectiveTool.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "GameFramework/Character.h"
#include "Mechanics/FragmentsLogic/FragmentedObject.h"
#include "Mechanics/FragmentsLogic/FragmentComponent.h"
#include "Mechanics/Perspective/CameraAlignmentComponent.h"
#include "Mechanics/Perspective/TriggerZone.h"

// UI
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Tools/PerspectiveTool/PerspectiveToolWidget.h"

class URegionComponent;


APerspectiveTool::APerspectiveTool()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APerspectiveTool::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacter = MakeWeakObjectPtr(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerCameraManager = MakeWeakObjectPtr(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
	PlayerController = MakeWeakObjectPtr(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	SetUpUI();

	if (PlayerController.IsValid())
	{
		PlayerController->SetShowMouseCursor(true);
		EnableInput(PlayerController.Get());
	}
}

void APerspectiveTool::SetUpUI()
{
	// crea widget e lo aggiunge alla viewport
	if (PerspectiveToolWidgetClass == nullptr) { return; }
	PerspectiveToolWidget = Cast<UPerspectiveToolWidget>(UWidgetBlueprintLibrary::Create(this, PerspectiveToolWidgetClass, nullptr));
	if (PerspectiveToolWidget == nullptr) { return; }
	PerspectiveToolWidget->SetPerspectiveTool(this);
	PerspectiveToolWidget->SetSliderValues(MinDistanceFromCamera, MaxDistanceFromCamera, MinDistanceFromCamera);
	PerspectiveToolWidget->AddToViewport(10);
}

void APerspectiveTool::SetCorrectPerspective()
{
	if (bMovedFragment) { return; }

	FVector FragmentedObjectLocation = FVector::ZeroVector;
	UCameraAlignmentComponent* CameraAlignmentComponent = nullptr;
	if (FragmentedObject.IsValid())
	{
		FragmentedObjectLocation = FragmentedObject->GetActorLocation();
		CameraAlignmentComponent = FragmentedObject->GetComponentByClass<UCameraAlignmentComponent>();
	}

	if (PlayerCharacter.IsValid())
	{
		PlayerLocation = PlayerCharacter->GetActorLocation();
	}

	if (PlayerCameraManager.IsValid())
	{
		CameraLocation = PlayerCameraManager->GetCameraLocation();
		CameraRotation = PlayerCameraManager->GetCameraRotation();
	}
	
	CameraDirection = UKismetMathLibrary::GetDirectionUnitVector(CameraLocation, FragmentedObjectLocation);
	InitialDistance = UKismetMathLibrary::Vector_Distance(CameraLocation, FragmentedObjectLocation);
	CheckDistance();

	if (IsValid(CameraAlignmentComponent))
	{
		CameraAlignmentComponent->SetCorrectViewDirection(CameraRotation);
	}

	if (TriggerZone.IsValid())
	{
		TriggerZone->SetPointOfViewLocation(PlayerLocation);
	}
		
	bPerspectiveSet = true;
}

void APerspectiveTool::CheckDistance()
{
	if (InitialDistance < MinDistanceFromCamera && PerspectiveToolWidget)
	{
		MinDistanceFromCamera = InitialDistance;
		PerspectiveToolWidget->SetSliderValues(MinDistanceFromCamera, MaxDistanceFromCamera, MinDistanceFromCamera);
	}
	if (InitialDistance > MaxDistanceFromCamera && PerspectiveToolWidget)
	{
		MaxDistanceFromCamera = InitialDistance;
		PerspectiveToolWidget->SetSliderValues(MinDistanceFromCamera, MaxDistanceFromCamera, MinDistanceFromCamera);
	}
}

void APerspectiveTool::OnMouseClick()
{
	if (!bPerspectiveSet) { return; }
	DeselectFragment();

	// get mouse location to world space
	FVector LineTraceStart = FVector::ZeroVector;
	FVector LineTraceDirection = FVector::ZeroVector;
	PlayerController->DeprojectMousePositionToWorld(LineTraceStart, LineTraceDirection);
	const FVector LineTraceEnd = LineTraceStart + (LineTraceDirection * 10000);
	
	// line trace
	FHitResult HitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(DefaultQueryParam), false);
	Params.AddIgnoredActor(PlayerCharacter.Get());
	Params.AddIgnoredActor(TriggerZone.Get());
	bool bHit = GetWorld()->LineTraceSingleByProfile(HitResult, LineTraceStart, LineTraceEnd, "BlockAllDynamic", Params);

	// se ha colpito qualcosa
	if (!bHit) { return; }
	const TWeakObjectPtr<UPrimitiveComponent> HitComponent = HitResult.Component;
	if (!HitComponent.IsValid()) { return; }

	// se il componente colpito è un componente del FragmentedObject (è un region component)
	if (HitComponent->GetOwner() == FragmentedObject)
	{
		USceneComponent* SelectedComponent = HitComponent->GetAttachParent();
		if (SelectedComponent == nullptr) { return; }
		SelectFragment(SelectedComponent);
	}
}

void APerspectiveTool::SelectFragment(USceneComponent* InSelectedComponent)
{
	UFragmentComponent* PSelectedFragment = Cast<UFragmentComponent, USceneComponent>(InSelectedComponent);
	if (PSelectedFragment == nullptr) { return; }
	SelectedFragment = MakeWeakObjectPtr(InSelectedComponent);
	if (!SelectedFragment.IsValid()) { return;}
	
	SetOverlayMaterialOnFragment(SelectionOverlayMaterial);

	const FVector FragmentLocation = SelectedFragment->GetComponentLocation();
	const float FragmentDistance = UKismetMathLibrary::Vector_Distance(CameraLocation, FragmentLocation);

	if (PerspectiveToolWidget != nullptr)
	{
		PerspectiveToolWidget->EnableSlider(true, FragmentDistance);
	}
}

void APerspectiveTool::DeselectFragment()
{
	if (!SelectedFragment.IsValid()) { return;}
	SetOverlayMaterialOnFragment(nullptr);
	SelectedFragment = nullptr;

	if (PerspectiveToolWidget != nullptr)
	{
		PerspectiveToolWidget->EnableSlider(false, MinDistanceFromCamera);
	}
}

void APerspectiveTool::SetOverlayMaterialOnFragment(UMaterialInterface* InOverlayMaterial) const
{
	TArray<USceneComponent*> FragmentChildren;
	SelectedFragment->GetChildrenComponents(false, FragmentChildren);

	for (USceneComponent* ChildComponent : FragmentChildren)
	{
		UStaticMeshComponent* Region = Cast<UStaticMeshComponent>(ChildComponent);
		if (Region == nullptr) { continue; }
		Region->SetOverlayMaterial(InOverlayMaterial);
	}
}

void APerspectiveTool::MoveAndScaleFragment(const float InDistance)
{
	if (!SelectedFragment.IsValid()) { return; }
	bMovedFragment = true;

	const FVector NewLocation = CameraLocation + (CameraDirection * InDistance);
	SelectedFragment->SetWorldLocation(NewLocation);

	const FVector NewScale = InDistance / FMath::Max(InitialDistance, KINDA_SMALL_NUMBER) * FVector::OneVector;
	SelectedFragment->SetWorldScale3D(NewScale);
}

void APerspectiveTool::MoveToCorrectView() const
{
	if (!bPerspectiveSet) { return; }

	if (!PlayerCharacter.IsValid()) { return; }
	PlayerCharacter->SetActorLocation(PlayerLocation);

	if (!PlayerController.IsValid()) { return; }
	PlayerController->SetControlRotation(CameraRotation);
}

void APerspectiveTool::OnPerspectificationCompleted()
{
	DeselectFragment();
}

#include "Mechanics/Shooting/ColorCasterPickup.h"

#include "Mechanics/Interactions/EquipWeaponComponent.h"
#include "Mechanics/Shooting/ColorCasterComponent.h"
#include "ProjectColor/ProjectColorCharacter.h"
#include "ProjectColor/ProjectColorFPAnim.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Mechanics/Shooting/ProjectileColorUtils.h"
#include "Mechanics/Shooting/Projectiles/ColorProjectile.h"

AColorCasterPickup::AColorCasterPickup()
{
	PrimaryActorTick.bCanEverTick = false;

	ColorCasterComponent = CreateDefaultSubobject<UColorCasterComponent>(TEXT("Color Caster"));
	ColorCasterComponent->SetupAttachment(RootComponent);
	ColorCasterComponent->SetCollisionProfileName(TEXT("NoCollision"));
	ColorCasterComponent->CanCharacterStepUpOn = ECB_No;
	ColorCasterComponent->SetGenerateOverlapEvents(false);
	ColorCasterComponent->SetEnableGravity(false);
	ColorCasterComponent->PrimaryComponentTick.bCanEverTick = false;

	EquipWeaponComponent = CreateDefaultSubobject<UEquipWeaponComponent>(TEXT("Equip Weapon"));
	EquipWeaponComponent->SetupAttachment(ColorCasterComponent);
	EquipWeaponComponent->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	EquipWeaponComponent->CanCharacterStepUpOn = ECB_No;
	EquipWeaponComponent->SetGenerateOverlapEvents(true);
	EquipWeaponComponent->SetEnableGravity(false);
	EquipWeaponComponent->SetSphereRadius(100.f);
	EquipWeaponComponent->PrimaryComponentTick.bCanEverTick = false;
	
	BrushPaintEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Brush Paint"));
	BrushPaintEffect->SetupAttachment(ColorCasterComponent);
	BrushPaintEffect->SetRelativeLocation(FVector(-45.f, 80.f, 0));
	BrushPaintEffect->Deactivate();
}

void AColorCasterPickup::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(ColorCasterComponent, TEXT("Color caster component not configured")))
	{
		ColorCasterComponent->OnProjectileColorChanged.AddDynamic(this, &AColorCasterPickup::OnWeaponProjectileChanged);

		// initialize niagara system color
		if (ColorCasterComponent->ProjectileOrder.Num() > 0)
		{
			UpdateBrushPaintVfxColor(ColorCasterComponent->ProjectileOrder[0]);
		}
	}

	if (ensureMsgf(EquipWeaponComponent, TEXT("Equip weapon component not configured")))
	{
		EquipWeaponComponent->OnPickUp.AddDynamic(this, &AColorCasterPickup::OnWeaponPickUp);
	}

	if (ensureMsgf(BrushPaintSystem, TEXT("Brush paint niagara system not configured")))
	{
		BrushPaintEffect->SetAsset(BrushPaintSystem);
		BrushPaintEffect->Activate();
	}
}

void AColorCasterPickup::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ColorCasterComponent && ColorCasterComponent->OnProjectileColorChanged.IsAlreadyBound(this, &AColorCasterPickup::OnWeaponProjectileChanged))
	{
		ColorCasterComponent->OnProjectileColorChanged.RemoveDynamic(this, &AColorCasterPickup::OnWeaponProjectileChanged);
	}

	if (EquipWeaponComponent && EquipWeaponComponent->OnPickUp.IsAlreadyBound(this, &AColorCasterPickup::OnWeaponPickUp))
	{
		EquipWeaponComponent->OnPickUp.RemoveDynamic(this, &AColorCasterPickup::OnWeaponPickUp);
	}
	
	Super::EndPlay(EndPlayReason);
}

FGameStateBlock AColorCasterPickup::FetchState() const
{
	const FPickableColorCasterState State = {bIsPickedUp};
	return {
		SaveBlockId,
		FInstancedStruct::Make(State)
	};
}

void AColorCasterPickup::ApplyState(const FGameStateBlock& Block)
{
	const FPickableColorCasterState& PickableColorCasterState = Block.Data.Get<FPickableColorCasterState>();
	if (!PickableColorCasterState.bIsPickedUp) { return; }

	bIsPickedUp = true;
	
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{
		AProjectColorCharacter* PCCharacter = Cast<AProjectColorCharacter>(PlayerCharacter);
		if (PCCharacter)
		{
			EquipWeaponComponent->Equip(PCCharacter);
		}
	}
}

void AColorCasterPickup::OnWeaponPickUp(AProjectColorCharacter* PickUpCharacter)
{
	if (!IsValid(PickUpCharacter) || !IsValid(PickUpCharacter->GetMesh1P())) { return;	}
	
	UAnimInstance* AnimInstance = PickUpCharacter->GetMesh1P()->GetAnimInstance();
	if (!IsValid(AnimInstance)) { return; }

	UProjectColorFPAnim* ProjectColorFPAnim = Cast<UProjectColorFPAnim>(AnimInstance);
	if (!IsValid(ProjectColorFPAnim)) { return;	}

	// change the anim machine state to show the rifle-grabbing position 
	ProjectColorFPAnim->HasRifle = true;

	bIsPickedUp = true;
}

void AColorCasterPickup::OnWeaponProjectileChanged(EProjectileColor ProjectileColor)
{
	UpdateBrushPaintVfxColor(ProjectileColor);
	UpdatePaintBrushTip(ProjectileColor);
}

void AColorCasterPickup::UpdateBrushPaintVfxColor(EProjectileColor ProjectileColor)
{
	if (!IsValid(BrushPaintEffect)) { return; }
	
	const FLinearColor VfxColor = UProjectileColorUtils::ToLinearColor(ProjectileColor);
	
	const FString ColorVariableStr = TEXT("Color");
	const FName ColorVariableName = FName(*ColorVariableStr);
	BrushPaintEffect->SetVariableLinearColor(ColorVariableName, VfxColor);
}

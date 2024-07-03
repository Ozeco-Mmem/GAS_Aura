// Copyright PandoraCube


#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
}

void AAuraEnemy::HighlightActor()
{
	// Highlight Mesh
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	// Highlight Weapon
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor()
{
	// Unhighlight Mesh
	GetMesh()->SetRenderCustomDepth(false);

	// Unhighlight Weapon
	Weapon->SetRenderCustomDepth(false);
}


void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Init ASC Actor<Server & Client> : Owner = Avatar = this 
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

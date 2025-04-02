// Fill out your copyright notice in the Description page of Project Settings.

#include "FirstPersonCharacter.h"
#include "Breakable.h"

// Sets default values
ABreakable::ABreakable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// Create the StaticMeshComponent
	BaseItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));

	// Set the StaticMeshComponent as the root component
	RootComponent = BaseItem;

	// Create the ChildActorComponent
	Breakables = CreateDefaultSubobject<UChildActorComponent>(TEXT("Breakables"));

	// Attach the ChildActorComponent to the StaticMeshComponent
	Breakables->SetupAttachment(BaseItem);

	Breakables->SetVisibility(false);
}

void ABreakable::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	BaseItem->SetStaticMesh(MeshAsset);

	if (isDescendent) {
		BaseItem->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BaseItem->SetSimulatePhysics(false);
		BaseItem->SetVisibility(false);
	}
	else {
		BaseItem->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		BaseItem->SetSimulatePhysics(true);
	}

	Breakables->GetChildrenComponents(false, BreakableComponents);

	for (USceneComponent* component : BreakableComponents) {
		if (UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(component)) {
			mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

// Called when the game starts or when spawned
void ABreakable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABreakable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// BREAKABLE ITEM IS NOT BECOMING VISIBLE UPON ACTIVATING
float ABreakable::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	AFirstPersonCharacter* player = Cast<AFirstPersonCharacter>(DamageCauser);
	FVector force = player->playerCamera->GetForwardVector() * 100000 * DamageAmount;
	BaseItem->AddForce(force);
	health -= DamageAmount;
	if (health <= 0) {
		BaseItem->SetVisibility(false);
		BaseItem->SetSimulatePhysics(false);
		BaseItem->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		for (USceneComponent* component : BreakableComponents) {
			UChildActorComponent* childComponent = Cast<UChildActorComponent>(component);
			ABreakable* breakable = Cast<ABreakable>(childComponent->GetChildActor());
			breakable->BaseItem->SetMassOverrideInKg(NAME_None, scrapMass, true);
			breakable->BaseItem->SetVisibility(true);
			//breakable->BaseItem->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			breakable->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			breakable->BaseItem->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			breakable->BaseItem->SetSimulatePhysics(true);
		}
	}
	return DamageAmount;
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Breakable.generated.h"

UCLASS()
class MOVEMENTPROJECT_API ABreakable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemState)
	bool isDescendent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemState)
	float scrapMass = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemState)
	float health = 100.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (ExposeOnSpawn = true))
	UStaticMesh* MeshAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* BaseItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UChildActorComponent* Breakables;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemState)
	TArray<USceneComponent *> BreakableComponents;

};

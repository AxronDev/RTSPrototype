// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

class ARTSPrototypeCharacter;
class USceneComponent;
class UDecalComponent;
class ABuilding;

UCLASS()
class RTSPROTOTYPE_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACameraPawn();

	/* void LookX(float value);
	void LookY(float value); */
	void Zoom(float value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Indicatior, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	UPROPERTY(replicated)
	UMaterialInstance* WhiteX;

	UPROPERTY(replicated)
	UMaterialInstance* RedX;

	void SetXLocation(FVector Loc);
	
	FVector XLoc;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AddGoldBuilding();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AddUnitBuilding();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AddHealthBuilding(ABuilding* New);

	UPROPERTY(replicated)
	TArray<AActor*> MyUnits;
	UPROPERTY(replicated)
	TArray<ABuilding*> MyHealthBuildings;

	UPROPERTY(BlueprintReadOnly, replicated)
	float Gold = 700;
	UPROPERTY(BlueprintReadOnly, replicated)
	float Units;
	UPROPERTY(BlueprintReadOnly, replicated)
	float TotalHealth = 500;

	float TempHealth = 0;

	UPROPERTY(EditAnywhere)
	float GoldPrice = 150.f;
	UPROPERTY(EditAnywhere)
	float UnitPrice = 100.f;
	UPROPERTY(EditAnywhere)
	float HealthPrice = 350.f;

private:

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere)
	float PanRate = 1500;

	FVector MoveDirection;

	UPROPERTY(replicated)
	int32 GoldBuildings = 1;
	UPROPERTY(replicated)
	int32 UnitBuildings = 0;
	UPROPERTY(replicated)
	int32 HealthBuildings = 0;

	float GoldRate = 3.f;
	float UnitRate = .5f;

	void MouseMovement(float DeltaTime);
};

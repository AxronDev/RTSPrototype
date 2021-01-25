// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RTSPrototypeCharacter.generated.h"

class AUnitAIController;
class UDamageType;

UENUM()
enum class ECharacterState : uint8
{
	Passive,
	Aggressive,
};

UCLASS(Blueprintable)
class ARTSPrototypeCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARTSPrototypeCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay();
	
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }
	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Indicatior, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;
	// Collision that checks if the character can spawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* SpawnSpace;

	bool bHasSpace = true;

	UFUNCTION(BlueprintCallable)
	float GetHealth();

	void Attack(AActor* Target);

	void SetOwnerUserName(FName UserName);

	UFUNCTION(BlueprintCallable)
	FName GetOwnerUserName();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ChangeCharacterState(ECharacterState NewState);

	UFUNCTION(BlueprintCallable)
	ECharacterState GetCharacterState();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<bool> AttackSlots;

    UPROPERTY(EditDefaultsOnly)
    float DistToTarget = 25.f; // come out to 78 cm away when stopped

    UPROPERTY(EditDefaultsOnly)
    float AttackDist = 120.f;

    UPROPERTY(EditDefaultsOnly)
    float AttackSpeed = 2.5f;

    UPROPERTY(EditDefaultsOnly)
    float AttackDamage = 5.f;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UDamageType> DamageTypeClass;

    bool bCanAttack = true;

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

protected:

	UPROPERTY(EditAnywhere, replicated)
	FName OwnerUserName = "";

private:
	UPROPERTY(replicated)
	ECharacterState CharacterState = ECharacterState::Passive;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_GetUnitController();

	UPROPERTY(replicated)
	AUnitAIController* UnitController;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UFUNCTION()
	void UpdateToOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void UpdateNotOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(replicated)
	float Health = 100;
};


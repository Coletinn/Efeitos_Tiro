// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arma.generated.h"

UCLASS()
class SURVIVALDEIMOS_API AArma : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArma();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* meta = (AllowPrivateAccess = "true") Indica que esta propriedade ou
	atributo estará acessível para acesso Blueprint 
	Significa que podem ser modificados na Blueprint
	Assim poderems modificar a MalhaDaArma por exemplo */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arma", meta =
		(AllowPrivateAcess = "true"))
	class USkeletalMeshComponent* MalhaDaArma;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arma")
	class UArrowComponent* SetaDaArma;

	UPROPERTY(EditAnywhere, Category = "Arma")
	class USoundBase* SomDoTiro;

	UPROPERTY(EditAnywhere, Category = "EfeitosDaArma")
	class UParticleSystem* EfeitoMuzzle;

	UPROPERTY(EditAnywhere, Category = "EfeitosImpacto")
	class UParticleSystem* ImpactoSangue;
	
	UPROPERTY(EditAnywhere, Category = "EfeitosImpacto")
	class UParticleSystem* ImpactoGeral;

	UPROPERTY(EditAnywhere, Category = "EfeitoImpacto")
	class UMaterialInterface* DecalImpactoGeral;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Atirar();

};

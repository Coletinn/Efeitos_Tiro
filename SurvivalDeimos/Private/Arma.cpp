// Fill out your copyright notice in the Description page of Project Settings.


#include "Arma.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/Engine.h"
#include "CollisionQueryParams.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/SkeletalMeshActor.h"
#include "Materials/MaterialInterface.h"
#include "Math/UnrealMathUtility.h"
#include "Sound/SoundBase.h"

// Sets default values
AArma::AArma()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EfeitoMuzzle = nullptr;
	ImpactoSangue = nullptr;
	ImpactoGeral = nullptr;
	DecalImpactoGeral = nullptr;
	SomDoTiro = nullptr;

	MalhaDaArma = CreateDefaultSubobject<USkeletalMeshComponent>(FName("MalhaDaArma"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshDaArma
	(TEXT("SkeletalMesh'/Game/Weapons/Rifle.Rifle'"));

	if (MeshDaArma.Succeeded())
	{
		MalhaDaArma->SetSkeletalMesh(MeshDaArma.Object);
	}

	RootComponent = MalhaDaArma;

	/* Componente SETA (Arrow Component)*/
	SetaDaArma = CreateDefaultSubobject<UArrowComponent>(FName("SetaCanoDaArma"));


	/* Aqui colocamos que este construtor seja anexado a MalhaDaArma
	Contudo é solicitado um attach para algo que ainda não foi configurado
	pois primeiro a classe C++ e executada e depois qualquer Blueprint que
	tem esta classe como pai. Assim o Mesh ou SkeletalMesh da arma ainda não
	estará disponível aqui pois está setado na Blueprint
	Logo precisamod configurar no código MalhaDaArma 
	Outra coisa importante e recomendada pela Unreal é que no construtor
	use a função SetupAttachment pata fazer o attach de algum componente
	a seu dono. Logo somente usar AttachToComponent fora do construtor*/
	SetaDaArma->SetupAttachment(MalhaDaArma, FName("MuzzleFlashSocket"));
	SetaDaArma->SetRelativeLocation(FVector(1.5f, 0.f, -1.2f));
	SetaDaArma->SetRelativeScale3D(FVector(0.3f, 0.8f, 0.7f));

}

// Called when the game starts or when spawned
void AArma::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArma::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AArma::Atirar()
{
	SetaDaArma = FindComponentByClass<UArrowComponent>();
	if (SetaDaArma != nullptr)
	{
		/* SetaDaArma é um componente do ator logo para achar sua localização
		utilizamos GetComponentLocation
		Se fosse a Arma seria GetActorLocation() pois a arma é um ator
		que possui um arrow(SetaDaArma) */
		FVector Inicio = SetaDaArma->GetComponentLocation();
		FVector Direcao = SetaDaArma->GetComponentRotation().Vector();
		FVector Fim = Inicio + (Direcao * 1000);

		/* O Raio de Raycast vai percorrer até o fim e precisa colocar as
		informações de HIT(impacto) em alguma estrutura. Essa estrutura é
		FHitResult*/
		FHitResult InfoImpacto;
		FCollisionQueryParams Parametros;
		/* Não queremos que o raio colida com a própria seta (this)*/
		Parametros.AddIgnoredActor(this);
		/* GetOwner devolve o dono da seta
		Neste caso devolve a malha - mesh da arm*/
		Parametros.AddIgnoredActor(GetOwner());
		Parametros.bTraceComplex = true;

		bool AcertouEmAlgo = GetWorld()->LineTraceSingleByChannel(InfoImpacto, Inicio, Fim,
			ECollisionChannel::ECC_Visibility, Parametros);

		if (SomDoTiro)
		{
			UGameplayStatics::PlaySoundAtLocation(SetaDaArma, SomDoTiro, Inicio);


		}

		if (AcertouEmAlgo)
		{
			UE_LOG(LogTemp, Warning, TEXT("Acertou em algo"));
			// Qual o ator que o raio impactou
			AActor* Ator = InfoImpacto.GetActor();

			/* Se a classe do ator que o raio atingiu for do tipo SkeletalMeshActor ou
			subclasses dela entre neste if. ImpactoSangue precisa ser válido */
			if (Ator->GetClass()->IsChildOf(ASkeletalMeshActor::StaticClass()) && ImpactoSangue)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactoSangue,
					InfoImpacto.Location, InfoImpacto.ImpactNormal.Rotation(), true);
			}
			/* Se não for um inimigo humanoide não queremos que seja sangue*/
			else if (ImpactoGeral)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactoGeral,
					InfoImpacto.Location, InfoImpacto.ImpactNormal.Rotation(), true);

				FVector TamanhoVariavel = FVector(FMath::RandRange(10.f, 50.f));
				UGameplayStatics::SpawnDecalAttached(DecalImpactoGeral, TamanhoVariavel,
					InfoImpacto.GetComponent(), NAME_None, InfoImpacto.Location,
					InfoImpacto.ImpactNormal.Rotation(), EAttachLocation::KeepWorldPosition, 60.f);
			}

		}

		//DrawDebugLine(GetWorld(), Inicio, Fim, FColor::Red, false, 5.0f, (uint8)0, 1.0f);

		if (EfeitoMuzzle)
		{

			FVector Localizacao = SetaDaArma->GetComponentLocation();
			FRotator Rotacao = SetaDaArma->GetComponentRotation();
			FVector Escala = FVector(0.9f);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EfeitoMuzzle,
				Localizacao, Rotacao, Escala, true);
		}
	}
}


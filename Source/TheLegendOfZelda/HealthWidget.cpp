// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthWidget.h"
#include "Components/Image.h"

void UHealthWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	M_Dynamic = Heart_Image->GetDynamicMaterial();
	M_Dynamic->SetScalarParameterValue(TEXT("Decimial"),0);
}

void UHealthWidget::SetHeartLevel(float Level)
{
	M_Dynamic->SetScalarParameterValue(TEXT("Decimal"),Level);
}
// Copyright Epic Games, Inc. All Rights Reserved.
#include "MaterialX/MaterialExpressionMinus.h"
#include "MaterialCompiler.h"

#define LOCTEXT_NAMESPACE "MaterialExpressionMinus"

UMaterialExpressionMinus::UMaterialExpressionMinus(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		FText NAME_MaterialX;
		FText NAME_Compositing;
		FConstructorStatics() :
			NAME_MaterialX(LOCTEXT("MaterialX", "MaterialX")),
			NAME_Compositing(LOCTEXT("Compositing", "Compositing"))
		{}
	};
	static FConstructorStatics ConstructorStatics;

#if WITH_EDITORONLY_DATA
	MenuCategories.Add(ConstructorStatics.NAME_MaterialX);
	MenuCategories.Add(ConstructorStatics.NAME_Compositing);
#endif
}

#if WITH_EDITOR
int32 UMaterialExpressionMinus::Compile(FMaterialCompiler* Compiler, int32 OutputIndex)
{
	if(!A.GetTracedInput().Expression)
	{
		return Compiler->Errorf(TEXT("Missing A input"));
	}

	if(!B.GetTracedInput().Expression)
	{
		return Compiler->Errorf(TEXT("Missing B input"));
	}

	int32 IndexAlpha = Alpha.GetTracedInput().Expression ? Alpha.Compile(Compiler) : Compiler->Constant(ConstAlpha);

	int32 IndexB = B.Compile(Compiler);
	int32 Sub = Compiler->Sub(A.Compile(Compiler), IndexB);
	return Compiler->Lerp(IndexB, Sub, IndexAlpha);
}

void UMaterialExpressionMinus::GetCaption(TArray<FString>& OutCaptions) const
{
	OutCaptions.Add(TEXT("Minus"));
}
#endif

#undef LOCTEXT_NAMESPACE 
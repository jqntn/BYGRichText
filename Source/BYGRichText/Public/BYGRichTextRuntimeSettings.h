// Copyright Brace Yourself Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Misc/DataValidation.h"

#include "BYGRichTextRuntimeSettings.generated.h"

class UBYGRichTextStylesheet;

UCLASS(config = Engine, defaultconfig)
class BYGRICHTEXT_API UBYGRichTextRuntimeSettings : public UObject
{
	GENERATED_BODY()

public:
	UBYGRichTextRuntimeSettings(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
		  , TagOpenCharacter("[")
		  , TagCloseCharacter("]")
		  , ParagraphSeparator("\r\n\r\n")
		  , FallbackFontPath("/Engine/EngineFonts/Roboto.Roboto")
	{
	}

	// Stylesheet selected by default in new BYG Rich Text Block widgets
	UPROPERTY(config, EditAnywhere, Category = Settings, meta = ( MetaClass = "/Script/BYGRichText.BYGRichTextStylesheet" ))
	FSoftClassPath DefaultStylesheet;

	UPROPERTY(config, EditAnywhere, Category = Settings)
	FString TagOpenCharacter;
	UPROPERTY(config, EditAnywhere, Category = Settings)
	FString TagCloseCharacter;

	UPROPERTY(config, EditAnywhere, Category = Settings)
	FString ParagraphSeparator;
	
	UPROPERTY(config, EditAnywhere, Category = Settings, meta = ( AllowedClasses = "/Script/Engine.Font", DisplayName="Fallback Font" ))
	FSoftObjectPath FallbackFontPath;

#if WITH_EDITOR
	EDataValidationResult IsDataValid(FDataValidationContext& Context) const override
	{		
		EDataValidationResult Result = Super::IsDataValid(Context);

		if (TagOpenCharacter.Len() <= 0)
		{
            Context.AddError(FText::FromString("Tag open character cannot be blank"));			
		}
		if (TagCloseCharacter.Len() <= 0)
		{
            Context.AddError(FText::FromString("Tag close character cannot be blank"));			
		}
		if (Context.GetNumErrors() > 0)
		{
			return EDataValidationResult::Invalid;
		}

		return Result;
	}
#endif
};

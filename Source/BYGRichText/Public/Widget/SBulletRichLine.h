#pragma once

#include "CoreMinimal.h"
#include "Widgets/Text/SRichTextBlock.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SBoxPanel.h"


class SBulletRichLine : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBulletRichLine) {}
		SLATE_ARGUMENT(TSharedRef<SWidget>, BulletWidget)
		SLATE_ARGUMENT(TSharedRef<SRichTextBlock>, RichText)
		SLATE_ARGUMENT(float, Spacing)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		RichText = InArgs._RichText;
		Spacing = InArgs._Spacing;

		RichText->SetAutoWrapText(true);
		RichText->SetWrappingPolicy(ETextWrappingPolicy::DefaultWrapping);

		ChildSlot
			[
				SNew(SHorizontalBox)

					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Top)
					[
						InArgs._BulletWidget
					]

					+ SHorizontalBox::Slot()
					.FillWidth(1.f)
					.VAlign(VAlign_Top)
					.Padding(FMargin(Spacing, 0, 0, 0))
					[
						RichText.ToSharedRef()
					]
			];
	}

	virtual void Tick(const FGeometry& AllottedGeometry, const double, const float) override
	{
		const float Total = AllottedGeometry.GetLocalSize().X;

		const FVector2D BulletSize =
			StaticCastSharedRef<SHorizontalBox>(ChildSlot.GetWidget())
			->GetChildren()->GetChildAt(0)->GetDesiredSize();

		const float Available = FMath::Max(0.f, Total - BulletSize.X - Spacing);

		if (!FMath::IsNearlyEqual(Available, LastWrapAt, 0.5f))
		{
			LastWrapAt = Available;
			RichText->SetWrapTextAt(Available);
		}
	}

private:
	TSharedPtr<SRichTextBlock> RichText;
	float Spacing = 0.f;
	float LastWrapAt = -1.f;
};

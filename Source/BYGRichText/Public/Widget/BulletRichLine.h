#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/SRichTextBlock.h"

class SBulletRichLine : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SBulletRichLine )
		: _BulletWidget( SNullWidget::NullWidget )
		, _ContentWidget( SNullWidget::NullWidget )
		, _Spacing( 0.f )
	{}
		SLATE_ARGUMENT( TSharedPtr<SWidget>, BulletWidget )
		SLATE_ARGUMENT( TSharedPtr<SWidget>, ContentWidget )
		SLATE_ARGUMENT( float, Spacing )
	SLATE_END_ARGS()

	void Construct( const FArguments& InArgs )
	{
		Spacing = InArgs._Spacing;

		ContentWidget = InArgs._ContentWidget;
		if ( !ContentWidget.IsValid() )	
		{
			ContentWidget = SNullWidget::NullWidget;
		}

		RichText = StaticCastSharedPtr<SRichTextBlock>( ContentWidget );
		if ( RichText.IsValid() )
		{
			RichText->SetAutoWrapText( true );
			RichText->SetWrappingPolicy( ETextWrappingPolicy::DefaultWrapping );
		}

		const TSharedRef<SWidget> BulletWidget = InArgs._BulletWidget.IsValid()
			? InArgs._BulletWidget.ToSharedRef()
			: SNullWidget::NullWidget;

		ChildSlot
		[
			SNew( SHorizontalBox )

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign( VAlign_Top )
			[
				BulletWidget
			]

			+ SHorizontalBox::Slot()
			.FillWidth( 1.f )
			.VAlign( VAlign_Top )
			.Padding( FMargin( Spacing, 0, 0, 0 ) )
			[
				ContentWidget.ToSharedRef()
			]	
		];
	}

	virtual void Tick( const FGeometry& AllottedGeometry, const double, const float ) override
	{
		if ( !RichText.IsValid() )
		{
			return;
		}

		const float Total = AllottedGeometry.GetLocalSize().X;

		const FVector2D BulletSize =
			StaticCastSharedRef<SHorizontalBox>( ChildSlot.GetWidget() )
			->GetChildren()->GetChildAt( 0 )->GetDesiredSize();

		const float Available = FMath::Max( 0.f, Total - BulletSize.X - Spacing );

		if ( !FMath::IsNearlyEqual( Available, LastWrapAt, 0.5f ) )
		{
			LastWrapAt = Available;
			RichText->SetWrapTextAt( Available );
		}
	}

private:
	TSharedPtr<SWidget> ContentWidget;
	TSharedPtr<SRichTextBlock> RichText;
	float Spacing = 0.f;
	float LastWrapAt = -1.f;
};

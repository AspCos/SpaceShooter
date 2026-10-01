#include "SpaceShooterHUDWidget.h"

#include "SpaceShooterPlayerController.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Fonts/SlateFontInfo.h"
#include "Kismet/KismetSystemLibrary.h"

void USpaceShooterHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (!WidgetTree)
	{
		return;
	}

	RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootCanvas"));
	WidgetTree->RootWidget = RootCanvas;

	StatsLabel = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("StatsLabel"));
	StatsLabel->SetText(FText::FromString(TEXT("SCORE 000000   VIES 3")));
	StatsLabel->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	FSlateFontInfo StatsFont;
	StatsFont.Size = 24;
	StatsLabel->SetFont(StatsFont);
	if (UCanvasPanelSlot* StatsSlot = RootCanvas->AddChildToCanvas(StatsLabel))
	{
		StatsSlot->SetAnchors(FAnchors(0.0f, 0.0f));
		StatsSlot->SetPosition(FVector2D(32.0f, 24.0f));
		StatsSlot->SetSize(FVector2D(500.0f, 48.0f));
	}

	MenuPanel = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("MenuPanel"));
	if (UCanvasPanelSlot* MenuSlot = RootCanvas->AddChildToCanvas(MenuPanel))
	{
		MenuSlot->SetAnchors(FAnchors(0.5f, 0.5f));
		MenuSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		MenuSlot->SetPosition(FVector2D::ZeroVector);
		MenuSlot->SetSize(FVector2D(460.0f, 360.0f));
	}

	UTextBlock* Title = CreateLabel(FText::FromString(TEXT("SPACE SHOOTER")), 38,
		FLinearColor(0.45f, 0.88f, 1.0f, 1.0f));
	if (UVerticalBoxSlot* TitleSlot = MenuPanel->AddChildToVerticalBox(Title))
	{
		TitleSlot->SetPadding(FMargin(0.0f, 18.0f, 0.0f, 22.0f));
		TitleSlot->SetHorizontalAlignment(HAlign_Center);
	}

	UTextBlock* TeamLabel = CreateLabel(FText::FromString(TEXT("Équipe : à compléter")), 16,
		FLinearColor(0.82f, 0.86f, 0.9f, 1.0f));
	if (UVerticalBoxSlot* TeamSlot = MenuPanel->AddChildToVerticalBox(TeamLabel))
	{
		TeamSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 24.0f));
		TeamSlot->SetHorizontalAlignment(HAlign_Center);
	}

	StartButton = CreateButton(FText::FromString(TEXT("Lancer le jeu")), MenuPanel);
	StartButton->OnClicked.AddDynamic(this, &USpaceShooterHUDWidget::OnStartClicked);
	QuitButton = CreateButton(FText::FromString(TEXT("Quitter")), MenuPanel);
	QuitButton->OnClicked.AddDynamic(this, &USpaceShooterHUDWidget::OnQuitClicked);

	GameOverPanel = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("GameOverPanel"));
	if (UCanvasPanelSlot* GameOverSlot = RootCanvas->AddChildToCanvas(GameOverPanel))
	{
		GameOverSlot->SetAnchors(FAnchors(0.5f, 0.5f));
		GameOverSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		GameOverSlot->SetSize(FVector2D(460.0f, 140.0f));
	}
	UTextBlock* GameOverLabel = CreateLabel(FText::FromString(TEXT("PARTIE TERMINÉE")), 34,
		FLinearColor(1.0f, 0.46f, 0.35f, 1.0f));
	if (UVerticalBoxSlot* GameOverLabelSlot = GameOverPanel->AddChildToVerticalBox(GameOverLabel))
	{
		GameOverLabelSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 24.0f));
		GameOverLabelSlot->SetHorizontalAlignment(HAlign_Center);
	}
	CreateButton(FText::FromString(TEXT("Quitter")), GameOverPanel)
		->OnClicked.AddDynamic(this, &USpaceShooterHUDWidget::OnQuitClicked);

	ShowMainMenu();
}

void USpaceShooterHUDWidget::ShowMainMenu()
{
	MenuPanel->SetVisibility(ESlateVisibility::Visible);
	StatsLabel->SetVisibility(ESlateVisibility::Hidden);
	GameOverPanel->SetVisibility(ESlateVisibility::Hidden);
}

void USpaceShooterHUDWidget::ShowGameplay()
{
	MenuPanel->SetVisibility(ESlateVisibility::Hidden);
	StatsLabel->SetVisibility(ESlateVisibility::Visible);
	GameOverPanel->SetVisibility(ESlateVisibility::Hidden);
}

void USpaceShooterHUDWidget::ShowGameOver()
{
	MenuPanel->SetVisibility(ESlateVisibility::Hidden);
	GameOverPanel->SetVisibility(ESlateVisibility::Visible);
}

void USpaceShooterHUDWidget::UpdateStats(int32 NewScore, int32 NewLives)
{
	if (StatsLabel)
	{
		StatsLabel->SetText(FText::FromString(FString::Printf(
			TEXT("SCORE %06d   VIES %d"), NewScore, NewLives)));
	}
}

void USpaceShooterHUDWidget::OnStartClicked()
{
	if (ASpaceShooterPlayerController* PlayerController = Cast<ASpaceShooterPlayerController>(GetOwningPlayer()))
	{
		PlayerController->StartGame();
	}
}

void USpaceShooterHUDWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}

UTextBlock* USpaceShooterHUDWidget::CreateLabel(const FText& Text, int32 FontSize,
	const FLinearColor& Color)
{
	UTextBlock* Label = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	Label->SetText(Text);
	Label->SetColorAndOpacity(FSlateColor(Color));
	FSlateFontInfo Font;
	Font.Size = FontSize;
	Label->SetFont(Font);
	return Label;
}

UButton* USpaceShooterHUDWidget::CreateButton(const FText& Text, UVerticalBox* Parent)
{
	UButton* Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());
	UTextBlock* Label = CreateLabel(Text, 20, FLinearColor::White);
	Button->AddChild(Label);
	if (UVerticalBoxSlot* ButtonSlot = Parent->AddChildToVerticalBox(Button))
	{
		ButtonSlot->SetPadding(FMargin(28.0f, 8.0f));
		ButtonSlot->SetHorizontalAlignment(HAlign_Fill);
	}
	return Button;
}
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpaceShooterHUDWidget.generated.h"

class UButton;
class UCanvasPanel;
class UTextBlock;
class UVerticalBox;

UCLASS()
class SPACESHOOTER_API USpaceShooterHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void ShowMainMenu();
	void ShowGameplay();
	void ShowGameOver();
	void UpdateStats(int32 NewScore, int32 NewLives);

private:
	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnQuitClicked();

	UTextBlock* CreateLabel(const FText& Text, int32 FontSize, const FLinearColor& Color);
	UButton* CreateButton(const FText& Text, UVerticalBox* Parent);

	UPROPERTY()
	TObjectPtr<UCanvasPanel> RootCanvas;

	UPROPERTY()
	TObjectPtr<UVerticalBox> MenuPanel;

	UPROPERTY()
	TObjectPtr<UVerticalBox> GameOverPanel;

	UPROPERTY()
	TObjectPtr<UTextBlock> StatsLabel;

	UPROPERTY()
	TObjectPtr<UButton> StartButton;

	UPROPERTY()
	TObjectPtr<UButton> QuitButton;
};
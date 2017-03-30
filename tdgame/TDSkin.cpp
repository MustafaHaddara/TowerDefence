#include "TDSkin.h"
#include "TDGame.h"


using namespace TDGame;

MainWindow::MainWindow() :
	Window("panels/Splash"),
	Global<MainWindow>(TheMainWindow)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::OpenWindow(void)
{
	if (TheMainWindow)
	{
		TheInterfaceMgr->SetActiveWindow(TheMainWindow);
	}
	else
	{
		TheGame->AddWindow(new MainWindow);
	}
}

void MainWindow::ReturnToGame(void)
{
	World *world = TheWorldMgr->GetWorld();
	if (world)
	{
		world->SetWorldFlags(world->GetWorldFlags() & ~kWorldPaused);
	}

	TheTimeMgr->SetWorldTimeMultiplier(1.0F);
}

void MainWindow::HandleSinglePlayerWindowClose(Window *window, void *cookie)
{
	OpenWindow();
}

void MainWindow::PreprocessWidget(void)
{
	Window::PreprocessWidget();

	newButton = static_cast<TextButtonWidget *>(FindWidget("New"));
	loadButton = static_cast<TextButtonWidget *>(FindWidget("Load"));
	saveButton = static_cast<TextButtonWidget *>(FindWidget("Save"));
	exitButton = static_cast<TextButtonWidget *>(FindWidget("Exit"));

	hostButton = static_cast<TextButtonWidget *>(FindWidget("Host"));
	joinButton = static_cast<TextButtonWidget *>(FindWidget("Join"));

	playerButton = static_cast<TextButtonWidget *>(FindWidget("Player"));
	controlsButton = static_cast<TextButtonWidget *>(FindWidget("Controls"));
	graphicsButton = static_cast<TextButtonWidget *>(FindWidget("Graphics"));
	audioButton = static_cast<TextButtonWidget *>(FindWidget("Audio"));

	creditsButton = static_cast<TextButtonWidget *>(FindWidget("Credits"));
	quitButton = static_cast<TextButtonWidget *>(FindWidget("Quit"));

	currentButton = nullptr;
	SetCurrentButton(newButton);

	World *world = TheWorldMgr->GetWorld();
	if (world)
	{
		if (!TheMessageMgr->GetMultiplayerFlag())
		{
			saveButton->EnableWidget();

			world->SetWorldFlags(world->GetWorldFlags() | kWorldPaused);
			TheTimeMgr->SetWorldTimeMultiplier(0.0F);
		}
	} else {
//		TheInterfaceMgr->GetStrip()->ShowWidget();
	}
}

void MainWindow::SetCurrentButton(TextButtonWidget *button)
{
	TextButtonWidget *previousButton = currentButton;
	if (previousButton)
	{
		Mutator *mutator = previousButton->GetFirstMutator();
		if (mutator)
		{
			mutator->SetMutatorState(kMutatorDisabled);
			previousButton->SetDynamicWidgetColor(previousButton->GetWidgetColor(kWidgetColorDefault));
		}

		Sound *sound = new Sound;
		sound->LoadSound("game/Menu");
		sound->PlaySound();
	}

	currentButton = button;

	Mutator *mutator = button->GetFirstMutator();
	if (mutator)
	{
		mutator->SetMutatorState(0);
	}
}

bool MainWindow::HandleKeyboardEvent(const KeyboardEventData *eventData)
{
	if (eventData->eventType == kEventKeyDown)
	{
		unsigned_int32 keyCode = eventData->keyCode;

		if (keyCode == kKeyCodeEscape)
		{
			delete this;
			ReturnToGame();
			return (true);
		}

		const World *world = TheWorldMgr->GetWorld();
		TextButtonWidget *button = currentButton;

		if (keyCode == kKeyCodeEnter)
		{
			if (button)
			{
				button->ActivateWidget();
			}

			return (true);
		}

		if (keyCode == kKeyCodeLeftArrow)
		{
			if (world)
				return(true);
			if (button == newButton)
			{
				SetCurrentButton(quitButton);
			}
			else if (button == quitButton)
			{
				SetCurrentButton(newButton);
			}			

			return (true);
		}

		if (keyCode == kKeyCodeRightArrow)
		{
			if (world)
				return(true);
			if (button == newButton)
			{
				SetCurrentButton(quitButton);
			}
			else if (button == quitButton)
			{
				SetCurrentButton(newButton);
			}

			return (true);
		}
	}

	return (Window::HandleKeyboardEvent(eventData));
}

void MainWindow::HandleWidgetEvent(Widget *widget, const WidgetEventData *eventData)
{
	if (eventData->eventType == kEventWidgetActivate)
	{
		Sound *sound = new Sound;
		sound->LoadSound("game/Select");
		sound->PlaySound();

		if (widget == newButton)
		{
            TheGame->HostMultiplayerGame("cap2", 0);
			delete this;
		}
		else if (widget == quitButton)
		{
			TheEngine->Quit();
		}
	}
}


void DisplayBoard::ShowMessageText(const char* text)
{
    myText->SetText(text);
}



DisplayBoard::DisplayBoard() : Global<DisplayBoard>(TheDisplayBoard)
{
    myText = new TextWidget(Vector2D(80.0F, 16.0F), "Test");
	myText->SetFont("fonts/berlinsans");
	myText->SetWidgetColor(ColorRGBA(1.0f, 1.0f, 1.0f)); // WHITE
	myText->SetWidgetPosition(Point3D(50, 50, 0));

	// Adds widget to the screen
	TheInterfaceMgr->AddWidget(myText);

	// New UI Code

	towerOne = new IconButtonWidget(Vector2D(64.0f, 64.0f), "TowerOne");
	towerOne->SetTexture(0, "textures/TowerButton1");
	towerOne->SetIconButtonFlags(kIconButtonSticky);
	//towerOne->SetWidgetColor(ColorRGBA(1.0f, 0.0f, 0.0f), kWidgetColorButton);
	TheInterfaceMgr->AddWidget(towerOne);

	towerTwo = new IconButtonWidget(Vector2D(64.0f, 64.0f), "TowerTwo");
	towerTwo->SetTexture(0, "textures/TowerButton2");
	towerTwo->SetIconButtonFlags(kIconButtonSticky);
	TheInterfaceMgr->AddWidget(towerTwo);

	towerThree = new IconButtonWidget(Vector2D(64.0f, 64.0f), "TowerThree");
	towerThree->SetTexture(0, "textures/TowerButton3");
	towerThree->SetIconButtonFlags(kIconButtonSticky);
	TheInterfaceMgr->AddWidget(towerThree);

	healthBackground = new ImageWidget(Vector2D(300.0f, 22.0f));
	healthBackground->SetWidgetKey("HealthBackground");
	healthBackground->SetTexture(0, "hud/ammo");

	healthProgress = new ProgressWidget(Vector2D(296.0f, 18.0f));
	healthProgress->SetWidgetKey("HealthBar");
	healthProgress->SetMaxValue(100);
	healthProgress->SetValue(100);
	healthProgress->SetWidgetColor(ColorRGBA(0.04f, 0.04f, 0.04f));

	towerImage = new ImageWidget(Vector2D(64.0f, 64.0f));
	towerImage->SetWidgetKey("TowerImage");
	towerImage->SetTexture(0, "textures/tower");

	moneyDisplay = new Widget();
	moneyDisplay->LoadPanel("panels/money");
	TheInterfaceMgr->AddWidget(moneyDisplay);

	moneyText = new TextWidget(Vector2D(128.0f, 16.0f), "5000", "fonts/berlinsans");
	moneyText->SetWidgetKey("Money");
	moneyText->SetTextAlignment(kTextAlignRight);

	AppendSubnode(towerOne);
	AppendSubnode(towerTwo);
	AppendSubnode(towerThree);
	AppendSubnode(healthBackground);
	AppendSubnode(healthProgress);
	AppendSubnode(towerImage);
	AppendSubnode(moneyDisplay);
	AppendSubnode(moneyText);

	UpdateDisplayPosition();
}

DisplayBoard::~DisplayBoard()
{
}



void DisplayBoard::OpenBoard(void)
{
    if (!TheDisplayBoard){
		 TheGame->AddBoard(new DisplayBoard);
    }
	TheDisplayBoard->UpdatePlayerHealth();
}





void DisplayBoard::Preprocess(void)
{

}

void DisplayBoard::MoveWidget(void)
{
	Board::MoveWidget();
}



void DisplayBoard::UpdateDisplayPosition(void)
{
	float displayWidth = float(TheDisplayMgr->GetDisplaySize().x);
    float displayHeight = float(TheDisplayMgr->GetDisplaySize().y);

	healthBackground->SetWidgetPosition(Point3D(
		displayWidth * 0.5F - 300.0F * 0.5F
		, 48.0F
		, 0.0f));

	healthProgress->SetWidgetPosition(Point3D(
		displayWidth * 0.5F - 296.0F * 0.5F
		, 50.0F
		, 0.0f));

	towerImage->SetWidgetPosition(Point3D(
		displayWidth * 0.5F - (64.0F + 300.0F) * 0.5F
		, 28.0F
		, 0.0f));

	towerOne->SetWidgetPosition(Point3D(
		displayWidth * 0.5F - 64.0F * 0.5F - 64.0F - 16.0F
		, displayHeight * 0.9F - 100.0F * 0.5F
		, 0.0F));

	towerTwo->SetWidgetPosition(Point3D(
		displayWidth * 0.5F - 64.0F * 0.5F
		, displayHeight * 0.9F - 100.0F * 0.5F
		, 0.0F));

	towerThree->SetWidgetPosition(Point3D(
		displayWidth * 0.5F - 64.0F * 0.5F + 64.0F + 16.0F
		, displayHeight * 0.9F - 100.0F * 0.5F
		, 0.0F));

	moneyDisplay->SetWidgetPosition(Point3D(
		50.0F
		, displayHeight * 0.9F - 64.0F * 0.5F
		, 0.0F));

	moneyText->SetWidgetPosition(Point3D(
		36.0F
		, displayHeight * 0.9F - 26.0F * 0.5F
		, 0.0F));
    
	InvalidateWidget();
}

void DisplayBoard::UpdatePlayerHealth(void)
{
	const Player *player = TheMessageMgr->GetLocalPlayer();
	if (player)
	{
		int32 health = 50;
		healthProgress = static_cast<ProgressWidget*>(FindWidget("HealthBar"));
		healthProgress->SetValue(health);
	}
}

void DisplayBoard::SelectTowerOne(void)
{
	if (towerOne->GetValue() == 0)
	{
		towerOne->SetValue(1);
		towerTwo->SetValue(0);
		towerThree->SetValue(0);
	}
	else
	{
		towerOne->SetValue(0);
	}
}

void DisplayBoard::SelectTowerTwo(void)
{
	if (towerTwo->GetValue() == 0)
	{
		towerTwo->SetValue(1);
		towerOne->SetValue(0);
		towerThree->SetValue(0);
	}
	else
	{
		towerTwo->SetValue(0);
	}
}

void DisplayBoard::SelectTowerThree(void)
{
	if (towerThree->GetValue() == 0)
	{
		towerThree->SetValue(1);
		towerOne->SetValue(0);
		towerTwo->SetValue(0);
	}
	else
	{
		towerThree->SetValue(0);
	}
}

DisplayBoard *TDGame::TheDisplayBoard = nullptr;
MainWindow *TDGame::TheMainWindow = nullptr;
SinglePlayerWindow *TDGame::TheSinglePlayerWindow = nullptr;

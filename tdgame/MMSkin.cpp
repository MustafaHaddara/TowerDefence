//
//  MMSkin.cpp
//



#include "MMSkin.h"
#include "MMGame.h"


using namespace MMGame;



DisplayBoard *MMGame::TheDisplayBoard = nullptr;



void DisplayBoard::ShowMessageText(const char* text)
{
    myText->SetText(text);
}



DisplayBoard::DisplayBoard() :  Global<DisplayBoard>(TheDisplayBoard)
{
        

    myText = new TextWidget(Vector2D(80.0F, 16.0F), "Test");
	myText->SetFont("font/Bold");
	myText->SetWidgetColor(ColorRGBA(1.0f, 1.0f, 1.0f)); // WHITE
	myText->SetWidgetPosition(Point3D(50, 50, 0));
    
    


	// Adds widget to the screen
	TheInterfaceMgr->AddWidget(myText);

	// New UI Code

	towerDisplay = new Widget();
	towerDisplay->LoadPanel("panels/towers");
	TheInterfaceMgr->AddWidget(towerDisplay);

	healthBackground = new ImageWidget(Vector2D(300.0f, 22.0f));
	healthBackground->SetWidgetKey("HealthBackground");
	healthBackground->SetTexture(0, "hud/ammo");

	healthProgress = new ProgressWidget(Vector2D(296.0f, 18.0f));
	healthProgress->SetWidgetKey("HealthBar");
	healthProgress->SetMaxValue(100);
	healthProgress->SetValue(100);
	healthProgress->SetWidgetColor(ColorRGBA(0.05f, 0.05f, 0.05f));

	towerImage = new ImageWidget(Vector2D(64.0f, 64.0f));
	towerImage->SetWidgetKey("TowerImage");
	towerImage->SetTexture(0, "textures/tower");

	moneyDisplay = new Widget();
	moneyDisplay->LoadPanel("panels/money");
	TheInterfaceMgr->AddWidget(moneyDisplay);

	moneyText = new TextWidget(Vector2D(128.0f, 16.0f), "4444", "fonts/berlinsans");
	moneyText->SetWidgetKey("Money");
	moneyText->SetTextAlignment(kTextAlignRight);

	AppendSubnode(towerDisplay);
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

	towerDisplay->SetWidgetPosition(Point3D(
		displayWidth * 0.5F - 512.0F * 0.5F
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
	// Test mutator functions

	const Player *player = TheMessageMgr->GetLocalPlayer();
	if (player)
	{
		int32 health = 50;
		healthProgress = static_cast<ProgressWidget*>(FindWidget("HealthBar"));
		healthProgress->SetValue(health);
		moneyText->SetText("5000");
	}
}



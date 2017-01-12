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
	

       //Invalidate();
}





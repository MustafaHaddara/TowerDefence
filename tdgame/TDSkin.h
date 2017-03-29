 //
//  TDSkin.h
//  C4
//
//  Created by Martin v. Mohrenschildt on 14-11-19.
//  Copyright (c) 2014 McMaster. All rights reserved.
//

#ifndef C4_TDSkin_h
#define C4_TDSkin_h

#include "TSInterface.h"





namespace TDGame
{
    
    using namespace Tombstone;

    
    class GameBoard : public Board, public ListElement<GameBoard>
    {
    protected:
        
        GameBoard(){};
        
    public:
        
        ~GameBoard(){};
    };

	class SinglePlayerWindow : public Window, public Global<SinglePlayerWindow>
	{
	private:

		SinglePlayerWindow();

	public:

		~SinglePlayerWindow();

		static void OpenWindow(void);

		void MoveWidget(void) override;
		void HandleWidgetEvent(Widget *widget, const WidgetEventData *eventData) override;
	};

    // Main title splash window
	class MainWindow : public Window, public Global<MainWindow>
	{
	private:

		TextButtonWidget		*newButton;
		TextButtonWidget		*loadButton;
		TextButtonWidget		*saveButton;
		TextButtonWidget		*exitButton;

		TextButtonWidget		*hostButton;
		TextButtonWidget		*joinButton;

		TextButtonWidget		*playerButton;
		TextButtonWidget		*controlsButton;
		TextButtonWidget		*graphicsButton;
		TextButtonWidget		*audioButton;

		TextButtonWidget		*creditsButton;
		TextButtonWidget		*quitButton;

		TextButtonWidget		*currentButton;

		MainWindow();

		static void HandleSinglePlayerWindowClose(Window *window, void *cookie);
		//static void HandleHostGameWindowClose(Window *window, void *cookie);
		//static void HandleJoinGameWindowClose(JoinGameWindow *window, void *cookie);

		void SetCurrentButton(TextButtonWidget *button);

	public:

		~MainWindow();

		static void OpenWindow(void);

		static void ReturnToGame(void);

		void PreprocessWidget(void) override;

		bool HandleKeyboardEvent(const KeyboardEventData *eventData) override;
		void HandleWidgetEvent(Widget *widget, const WidgetEventData *eventData) override;
	};

    class DisplayBoard : public GameBoard , public Global<DisplayBoard>
	{
    private:
        
               
        TextWidget                      *scoreText;
        TextWidget                      *healthText;
        
    
        
        Widget                          healthGroup;
        Widget                          scoreGroup;
        Widget                          weaponsGroup;

		TextWidget                      *myText;

		// Modifiable Widgets

		IconButtonWidget				*towerOne;
		IconButtonWidget				*towerTwo;
		IconButtonWidget				*towerThree;
		ImageWidget						*healthBackground;
		ImageWidget						*towerImage;
		ProgressWidget					*healthProgress;
		TextWidget						*moneyText;

		// Static Widgets

		//Widget							*towerDisplay;
		Widget							*moneyDisplay;
        
        static ColorRGBA CalculateBarColor(float value);
        
        
    public:
        
        DisplayBoard();
        ~DisplayBoard();
        static void OpenBoard(void);
        
        void Preprocess(void);
        void MoveWidget(void);
        
        void UpdateDisplayPosition(void);
		void UpdatePlayerHealth(void);
		void SelectTowerOne(void);
		void SelectTowerTwo(void);
		void SelectTowerThree(void);
        
         void ShowMessageText(const char* text);
        
    };

    extern DisplayBoard *TheDisplayBoard;
	extern MainWindow *TheMainWindow;
	extern SinglePlayerWindow *TheSinglePlayerWindow;
    
    
};
  


#endif

#ifndef C4_TDSkin_h
#define C4_TDSkin_h

#include "TSInterface.h"

namespace TDGame
{
    
    using namespace Tombstone;


	/**
	* \brief Base Invaders UI board.
	*/
    class GameBoard : public Board, public ListElement<GameBoard>
    {
    protected:

        GameBoard(){};
        
    public:

		/**
		* \brief Destructor
		*/
        ~GameBoard(){};
    };

	/**
	* \brief Game window for single player mode.
	*/
	class SinglePlayerWindow : public Window, public Global<SinglePlayerWindow>
	{
	private:

		SinglePlayerWindow();

	public:

		/**
		* \brief Destructor
		*/
		~SinglePlayerWindow();

		/**
		* \brief Displays the window
		*/
		static void OpenWindow(void);

		void MoveWidget(void) override;
		/**
		* \brief Handle interaction with the window
		*/
		void HandleWidgetEvent(Widget *widget, const WidgetEventData *eventData) override;
	};

	/**
	* \brief Main title splash window.
	*/
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

		/**
		* \brief Default constructor
		*/
		MainWindow();

		/**
		* \brief Handle the window being closed
		*/
		static void HandleSinglePlayerWindowClose(Window *window, void *cookie);

		/**
		* \brief Set the button that is currently selected
		*/
		void SetCurrentButton(TextButtonWidget *button);

	public:
		/**
		* \brief Destructor
		*/
		~MainWindow();

		/**
		* \brief Display the window
		*/
		static void OpenWindow(void);
		/**
		* \brief Resume from pausing the game
		*/
		static void ReturnToGame(void);

		void PreprocessWidget(void) override;

		bool HandleKeyboardEvent(const KeyboardEventData *eventData) override;
		void HandleWidgetEvent(Widget *widget, const WidgetEventData *eventData) override;
	};

	/**
	* \brief In-game HUD UI.
	*/
    class DisplayBoard : public GameBoard , public Global<DisplayBoard>
	{
    private:
		// Server/client text
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

		Widget							*moneyDisplay;

		/**
		* \brief Get a color corresponding to a value for progress bars
		*/
        static ColorRGBA CalculateBarColor(float value);
        
        
    public:

		/**
		* \brief Default constructor
		*/
        DisplayBoard();
		/**
		* \brief Destructor
		*/
        ~DisplayBoard();
		/**
		* \brief Display the HUD
		*/
        static void OpenBoard(void);
        
        void Preprocess(void);
        void MoveWidget(void);
        
		/**
		* \brief Update HUD widget positions
		*/
        void UpdateDisplayPosition(void);

		/**
		* \brief Update player health bar to match player health
		*/
		void UpdatePlayerHealth(void);
		/**
		* \brief Activate tower 1 button
		*/
		void SelectTowerOne(void);
		/**
		* \brief Activate tower 2 button
		*/
		void SelectTowerTwo(void);
		/**
		* \brief Activate tower 3 button
		*/
		void SelectTowerThree(void);

		/**
		* \brief Display text on the HUD
		* \param text The text to be displayed
		*/
         void ShowMessageText(const char* text);
        
    };

    extern DisplayBoard *TheDisplayBoard;
	extern MainWindow *TheMainWindow;
	extern SinglePlayerWindow *TheSinglePlayerWindow;
    
};
  


#endif

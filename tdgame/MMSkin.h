#ifndef C4_MMSkin_h
#define C4_MMSkin_h

#include "TSInterface.h"

namespace BaseInvaders
{
    
    using namespace Tombstone;

    
    class GameBoard : public Board, public ListElement<GameBoard>
    {
    protected:
        
        GameBoard(){};
        
    public:
        
        ~GameBoard(){};
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

		ImageWidget						*healthBackground;
		ImageWidget						*towerImage;
		ProgressWidget					*healthProgress;
		TextWidget						*moneyText;

		// Static Widgets

		// towerDisplay to be split into modifiable IconButtonWidgets
		Widget							*towerDisplay;
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
        
         void ShowMessageText(const char* text);
        
    };

    extern DisplayBoard *TheDisplayBoard;

    
    
    
};
  


#endif

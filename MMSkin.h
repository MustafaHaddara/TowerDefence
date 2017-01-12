 //
//  MMSkin.h
//  C4
//
//  Created by Martin v. Mohrenschildt on 14-11-19.
//  Copyright (c) 2014 McMaster. All rights reserved.
//

#ifndef C4_MMSkin_h
#define C4_MMSkin_h

#include "TSInterface.h"





namespace MMGame
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
        
        static ColorRGBA CalculateBarColor(float value);
        
        
    public:
        
        DisplayBoard();
        ~DisplayBoard();
        static void OpenBoard(void);
        
        void Preprocess(void);
        void MoveWidget(void);
        
        void UpdateDisplayPosition(void);
        
        
         void ShowMessageText(const char* text);
        
    };

    extern DisplayBoard *TheDisplayBoard;

    
    
    
};
  


#endif

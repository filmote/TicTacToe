#include <Arduboy2.h>
#include <ArduboyTones.h>

const uint8_t PROGMEM cross[] = {
16, 16,
0x04, 0x0E, 0x1F, 0x3E, 0x7C, 0xF8, 0xF0, 0xE0, 0xE0, 0xF0, 0xF8, 0x7C, 0x3E, 0x1F, 0x0E, 0x04,
0x20, 0x70, 0xF8, 0x7C, 0x3E, 0x1F, 0x0F, 0x07, 0x07, 0x0F, 0x1F, 0x3E, 0x7C, 0xF8, 0x70, 0x20,
};

const uint8_t PROGMEM nought[] = {
16, 16,
0xE0, 0xF8, 0xFC, 0xFE, 0x3E, 0x1F, 0x0F, 0x0F, 0x0F, 0x0F, 0x1F, 0x3E, 0xFE, 0xFC, 0xF8, 0xE0,
0x07, 0x1F, 0x3F, 0x7F, 0x7C, 0xF8, 0xF0, 0xF0, 0xF0, 0xF0, 0xF8, 0x7C, 0x7F, 0x3F, 0x1F, 0x07,
};

const uint16_t winner[] PROGMEM = {
NOTE_FS5,498, NOTE_REST,144, NOTE_FS5,34, NOTE_REST,72, NOTE_FS5,34, NOTE_REST,72, NOTE_FS5,177,
NOTE_REST,108, NOTE_FS5,105, NOTE_REST,36, NOTE_FS5,70, NOTE_REST,72, NOTE_GS5,105, NOTE_REST,36, NOTE_AS5,105,
NOTE_REST,36, NOTE_B5,1034, NOTE_REST,251, NOTE_B5,105, NOTE_REST,36, NOTE_CS6,70, NOTE_REST,72, NOTE_DS6,70,
NOTE_REST,72, NOTE_F6,605, NOTE_REST,36, NOTE_F6,34, NOTE_REST,72, NOTE_F6,34, NOTE_REST,72, NOTE_F6,34,
NOTE_REST,179, NOTE_CS6,177, NOTE_REST,36, NOTE_F6,70, NOTE_REST,72, NOTE_FS6,117, NOTE_REST,24, NOTE_GS6,105,
NOTE_REST,36, NOTE_FS6,1641,
TONES_END
};


const uint16_t playTurn[] PROGMEM = {
  NOTE_C3, 100, NOTE_A4, 25,
  NOTE_B4, 40, NOTE_A4, 15,
  TONES_END
};

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

byte board[3][3];

byte whosTurn = 1;
byte playerX = 1;
byte playerY = 1;
byte showWinner = 0;
byte player1Counter = 0;
byte player2Counter = 0;

// ---------------------------------------------------------------------------------------------

void WeHaveAWinner() {
  
  showWinner = whosTurn;
  sound.tones(winner);
  
  if (whosTurn == 1) { player1Counter = player1Counter + 1; }
  if (whosTurn == 2) { player2Counter = player2Counter + 1; }

}

// ---------------------------------------------------------------------------------------------

void setup() {
  
  arduboy.begin();

}

// ---------------------------------------------------------------------------------------------

void loop() {

  arduboy.clear();
  arduboy.pollButtons();


  // Draws the lines ..
  
  arduboy.fillRect(19,0, 2, 64, WHITE);
  arduboy.fillRect(41,0, 2, 64, WHITE);
  arduboy.fillRect(0,19, 64, 2, WHITE);
  arduboy.fillRect(0,41, 64, 2, WHITE);


  // Draw existing noughts and crosses ..
  
  for (int x = 0; x < 3 ; x = x + 1) {

    for (int y = 0; y < 3 ; y = y + 1) {

      if (board[x][y] == 1) {

        Sprites::drawOverwrite(1 + (x * 22), 1 + (y * 22), cross, 0);

      }

      if (board[x][y] == 2) {

        Sprites::drawOverwrite(1 + (x * 22), 1 + (y * 22), nought, 0);

      }

    }
  
  }
  
  
  // Draws cursor ..  
  
  arduboy.drawRect((playerX * 22), (playerY * 22), 18, 18);



  // Who's turn?

  if (whosTurn == 1) {
    arduboy.setCursor(80, 0);
    arduboy.print("Player 1");
  }
  else {
    arduboy.setCursor(80, 0);
    arduboy.print("Player 2");
  }

  arduboy.setCursor(86, 56);
  arduboy.print(player1Counter);
  arduboy.print(" - ");
  arduboy.print(player2Counter);
  


  // Handle players buttons ..

  if (showWinner == 0) {
    
    if (arduboy.justPressed(LEFT_BUTTON) and playerX > 0)   { playerX = playerX - 1; }
    if (arduboy.justPressed(RIGHT_BUTTON) and playerX < 2)  { playerX = playerX + 1; }
  
    if (arduboy.justPressed(UP_BUTTON) and playerY > 0)     { playerY = playerY - 1; }
    if (arduboy.justPressed(DOWN_BUTTON) and playerY < 2)   { playerY = playerY + 1; }
  
    if (arduboy.justPressed(A_BUTTON)) {
      
      if (board[playerX][playerY] == 0)   { 
  
        board[playerX][playerY] = whosTurn;
        sound.tones(playTurn); 

  
        // Check columns to see if player has won ?
        
        for (int x = 0; x < 3 ; x = x + 1) {
          
          if (board[x][0] == whosTurn and board[x][1] == whosTurn and board[x][2] == whosTurn) {
            
            WeHaveAWinner();

          }
          
        }


        // Check rows to see if player has won ?

        for (int y = 0; y < 3 ; y = y + 1) {
          
          if (board[0][y] == whosTurn and board[1][y] == whosTurn and board[2][y] == whosTurn) {

            WeHaveAWinner();

          }
          
        }


        // Check diagonals to see if player has won ?

        if (board[0][0] == whosTurn and board[1][1] == whosTurn and board[2][2] == whosTurn) {

            WeHaveAWinner();

        }

        if (board[0][2] == whosTurn and board[1][1] == whosTurn and board[2][0] == whosTurn) {

            WeHaveAWinner();

        }



        // Is it a draw ?

        if (showWinner == 0) {

          showWinner = 3;

          for (int x = 0; x < 3 ; x = x + 1) {
    
            for (int y = 0; y < 3 ; y = y + 1) {
        
              if (board[x][y] == 0) { showWinner = 0; }
        
            }
          
          }

        }


        // If the current player has not won then it's the other players turn ..
        
        if (showWinner == 0) { 

          if (whosTurn == 1) {
            whosTurn = 2;
          }
          else {
            whosTurn = 1;
          }
             
        }
  
      }
      else {
        
        arduboy.digitalWriteRGB(RED_LED, RGB_ON);
        arduboy.delayShort(100);
        arduboy.digitalWriteRGB(RED_LED, RGB_OFF);
  
      }
      
      
    }

  }
    

  // Should we show the winner or draw message ?
  
  if (showWinner != 0) {

    if (showWinner == 1 or showWinner == 2) {
      
      arduboy.setCursor(80, 12);
      arduboy.print("Winner !");
  
      if (showWinner == 1) { Sprites::drawOverwrite(93, 30, cross, 0); }
      if (showWinner == 2) { Sprites::drawOverwrite(93, 30, nought, 0); }

    }
    else {
      
      arduboy.setCursor(80, 12);
      arduboy.print(" Draw !");

    }

    if (arduboy.justPressed(B_BUTTON)) {

      showWinner = 0;


      // Clear the board ..
      
      for (int x = 0; x < 3 ; x = x + 1) {

        for (int y = 0; y < 3 ; y = y + 1) {
    
          board[x][y] = 0;
    
        }
      
      }


      // The loser can start the next game ..
      
      if (whosTurn == 1) {
        whosTurn = 2;
      }
      else {
        whosTurn = 1;
      }
      
    }
    
  }


  // Update the screen ..

  arduboy.display();
  
}


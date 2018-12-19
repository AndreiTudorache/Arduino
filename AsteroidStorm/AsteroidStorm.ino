#include "LedControl.h" 
#include "JoiceStick.h"
#include "SpaceShip.h"
#include "PushButton.h"
#include "ShipLasers.h"
#include "Asteroids.h"
#include "LeCeDe.h"
#include "MillisWait.h"


LedControl lc = LedControl(6,5,4,1); //DIN, CLK, LOAD, No. DRIVER


JoiceStick Ctrl;
SpaceShip Sheep;
Matrix8x8 Mat;
ShipLasers Laz;
PushButton bLf(2), bRg(3);
Asteroids myAst;
LeCeDe LCD(12,11,10,9,8,7);

MillisWait ragazAfisaj(500);
MillisWait diffRaise(10000);

/*~~Pentru Joc~~*/

void mvSheepLeft () { Sheep.moveLeft(); }
void mvSheepRight () { Sheep.moveRight(); }

void shootLeft() { Laz.addLaser ( Sheep.leftGun() ); }
void shootRight() { Laz.addLaser ( Sheep.rightGun() ); }


int lives = 3;

void initGame()
{
  Laz.clearMatrix();
  myAst.clearMatrix();

  Laz.setDelay(70);
  
  bLf.setAction (shootLeft);
  bRg.setAction (shootRight);
  
  bLf.setDelay(150);
  bRg.setDelay(190);

  Ctrl.initH ( mvSheepLeft, mvSheepRight);
  Ctrl.setDelay(120);

  LCD.setString(0,"     Score: ");
  LCD.setString(1,"                ");
}

int cevaScor = 0;
bool gameOver = false;
bool myMenu = true;

void Replay()
{
    gameOver = false;
    initGame();
}
void Exit()
{
    LCD.setFromPos(0,4,"EXIT");
    Mat.clearMatrix();
    myMenu = true;
}

void theActualGame() {
    
    
    myAst.setDiff(lives);

    Ctrl.checkH();
    bLf.onPress();
    bRg.onPress();
    Laz.update();

    Laz.contact(myAst,cevaScor);
    myAst.addMeteors(gameOver);
    Sheep.didCollide(gameOver,myAst);

    if(gameOver)
    {
  bLf.setAction (Replay);
  bRg.setAction (Exit);
    }

    char sirPtNumar[10];
    itoa(cevaScor,sirPtNumar,10);
    LCD.setFromPos(1,5,sirPtNumar);

    Mat = Sheep | Laz | myAst;
}



/*~~Pentru Meniu~~*/

char stateNames [][17] =  {
                "##   Meniu    ##",
                "## Dificultate##",
                "## ScorRecord ##"
                    };

char subStateNames [][17] = {
                "Start Joc.",
                "Dificultate",
                "ScorRecord",
                    };

char Dificultati [][7] = {
                "Usor.",
                "Mediu.",
                "Greu."
                    };


short theState = 0;
short cursor = 0;
bool outOfGame = true;


void plsCrs ()
{
    cursor = (cursor+1) % 3;
}

void mnCrs ()
{
    if(cursor > 0) cursor--;
}


void stateChangeInit()
{
    Ctrl.resetVars();
    Ctrl.initV(mnCrs,plsCrs);
}

/*~~GameOver~~*/
byte anX[] = {  B01000010,
                B00100100,
                B00011000,
                B00011000,
                B00100100,
                B01000010,
                B00000000,
                B00000000};

Matrix8x8 theX;


void gameOverAnimation()
{
    theX = anX;
    if(lives > 0) 
    {
    LCD.setString(0,"You died        ");
    LCD.setString(1,"Lives left:     "); 
    char auxMat[17];
    itoa(lives,auxMat,10);
    LCD.setFromPos(1,13,auxMat);
    gameOver = false;
    }
    else
    {
    LCD.setString(0,"#     GAME     #");
    LCD.setString(1,"##    OVER!   ##");
    myMenu = true;
    lives = 3;
    }
    LCD.printStrings();
    Mat = Sheep | theX;
    Mat.playOn(lc);
    delay(2000);
}

/*~~~SETUP~~~*/

void setup()
{
 // Serial.begin(9600);

  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 0); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen

  initGame();
}



void loop()
{
MillisWait::readTime();

if(myMenu)
{
    LCD.setString(0,"Asteroid Storm");
    LCD.setString(1,"Press to START");

    if(bLf.isPressed() || bRg.isPressed()) 
    {
        myMenu = false;
        gameOver = false;
        initGame();
    }
}
else
    {
        if(gameOver)
        {
            cevaScor = 0;
            lives--;
            gameOverAnimation();
            if(lives > 0) initGame();
            if(lives == 3 && myMenu ) Mat.clearMatrix();
        }
        else
            theActualGame();
    }

    if(ragazAfisaj.isOk()) LCD.printStrings();
    Mat.playOn(lc);

}
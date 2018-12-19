#include "ShipLasers.h"

#include <avr/pgmspace.h>
#include <Arduino.h>

#define zerO B00000000
#define DEF_DELAY 50

ShipLasers :: ShipLasers () 
{
  laserTracker = zerO;
  lazWait.setDelay(DEF_DELAY);
}

void ShipLasers::update()
{
  if ( (laserTracker != zerO) && lazWait.isOk())
    {
        laserTracker = laserTracker >> 1;
        shUp();
    }
}

void ShipLasers::addLaser (byte lasPos)
{
  octaMat [ 6 ] = lasPos;
  laserTracker = laserTracker | B01000000;
}

int ShipLasers::howMuchOnes ( byte toDisecate )
{
  byte aMask = 1;
  int theOnes = 0;
  while( toDisecate != zerO)
  {
    if( (aMask & toDisecate) != zerO) theOnes++;
    toDisecate >>= 1;
  }
  return theOnes;
}

void ShipLasers::contact(Matrix8x8& asteroMat, int& theScore)
{
  for ( int i = 0; i < 7; i ++ )
   {
    byte meetUp = octaMat [ i ] & asteroMat [ i ];
    if ( meetUp != zerO)
      {
        asteroMat [i] ^= octaMat [i];
        octaMat [i] -= meetUp;
        theScore += howMuchOnes(meetUp);
      }
   }
}

void ShipLasers::setDelay(unsigned long aDelay)
{
  lazWait.setDelay (aDelay) ;
}


void ShipLasers::resetDelay() 
{
  lazWait.setDelay (DEF_DELAY);
}

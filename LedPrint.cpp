#include "LedPrint.h"

LedPrint::LedPrint ( LedControl *existingControl, uint8_t firstDigitIndex, uint8_t numberOfDigits, uint8_t displayIsBackwards)
{
  _lc = existingControl;
  _firstDigitIndex = firstDigitIndex;
  _numberOfDigits  = numberOfDigits;
  _displayIsBackwards = displayIsBackwards;
}

LedPrint::LedPrint (uint8_t dataPin, uint8_t clkPin, uint8_t csPin, uint8_t numDigits, uint8_t displayIsBackwards)
{
  _displayIsBackwards = displayIsBackwards;
  
  _lc = new LedControl(dataPin, clkPin, csPin, numDigits/8 + (numDigits%8?1:0));
  
  /*   
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  shutdown(false);
    
  /* and clear the display */
  clearEntireDisplay();
 
  /* Set the brightness to a medium values */
  setIntensity(2);
  
  _firstDigitIndex = 0;
  _numberOfDigits  = numDigits;
}

LedPrint LedPrint::getSubDisplay(uint8_t firstDigitIndex, uint8_t numberOfDigits)
{
  return LedPrint(_lc, firstDigitIndex, numberOfDigits, _displayIsBackwards);
}


LedPrintJustifiable LedPrint::getJustifiedSubDisplay(uint8_t firstDigitIndex, uint8_t numberOfDigits)
{
  return LedPrintJustifiable(_lc, firstDigitIndex, numberOfDigits, _displayIsBackwards);
}

void LedPrint::clear()
{
  currentPos = 0;
  lastByte   = 0;
  for(uint8_t x = 0; x < _numberOfDigits; x++)
  {
    write(' ');
  }
  currentPos = 0;
}

size_t LedPrint::write(uint8_t c)
{
  // Signal that the next time we print, clear
  if(c == '\n' || c == '\r')
  {
    lastByte = '\n';
    return 1;
  }
  
  // Anything else below space, ignore
  if(c < 32)
  {
    return 1;
  }
  
  // Clear display if the last time was a println
  if(lastByte == '\n')
  {
    clear();
  }
  
  uint8_t dp = false;
  // For a decimal point, we need to set the point on the preceding index (if any)
  // not the current index
  
  if((char) c == '.')
  {
    dp = true;
    if(!lastWasDP && lastByte)
    {
      c = lastByte; 
      currentPos--;
    }
    else
    {
      c = ' ';
    }
  }
  
  _printCharacter(currentPos, c, dp);
  
  
  // Save this in case we have to modify it to add a decimal point next
  lastByte = c;
  lastWasDP = dp;
  currentPos++;
    
  //currentPos = currentPos ==  _numberOfDigits-1 ? 0 : currentPos+1;
  
  return 1;
}

void LedPrint::_printCharacter(uint8_t &currentPos, uint8_t c, uint8_t dp)
{
  // If we have run off the end of the (sub)display, do not actually print
  //  anything, just pretend we did
  if(currentPos < _numberOfDigits)
  {
    setChar( _firstDigitIndex + currentPos, c, dp);
  }
}

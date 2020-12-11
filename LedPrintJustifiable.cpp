#include "LedPrintJustifiable.h"

LedPrintJustifiable::LedPrintJustifiable ( LedControl *existingControl, uint8_t firstDigitIndex, uint8_t numberOfDigits, uint8_t displayIsBackwards, int8_t Justification)
{
  _lc = existingControl;
  _firstDigitIndex = firstDigitIndex;
  _numberOfDigits  = numberOfDigits;
  _displayIsBackwards = displayIsBackwards;
  justify(Justification);
}

int8_t LedPrintJustifiable::justify(int8_t Justification, char blank)
{
  _justificationBlank = blank < 10 ? blank+48 : blank;
  
  // Left
  if(Justification == -1)
  {
    if(justifyBuffer)
    {
      free(justifyBuffer);
      justifyBuffer = NULL;
    }
    _justification = -1;
  }
  else
  {
    if(!justifyBuffer)
    {
      justifyBuffer = (uint8_t *)malloc(_numberOfDigits);
    }
    
    if(!justifyBuffer)
    {
      _justification = -1;
    }
    else
    {
      _justification = Justification;
    }
  }
  
  return _justification;
}

void LedPrintJustifiable::_printCharacter(uint8_t &currentPos, uint8_t c, uint8_t dp)
{
  if(justifyBuffer)
  {
    // If we have run off the end of the (sub)display, shift the current justify buffer along
    while(currentPos >= _numberOfDigits)
    {
      for(uint8_t x = 0; x < _numberOfDigits-1; x++)
      {
        *(justifyBuffer+x) = *(justifyBuffer+x+1);
      }
      currentPos--;
    }
    
    *(justifyBuffer+currentPos) = c | (dp?0b10000000:0);
    
    // how many empty digits have we?
    uint8_t empties = _numberOfDigits - (currentPos+1);
    uint8_t jPos = 0;
    
    // insert empties on the left
    uint8_t leading = (_justification==1?empties:(empties/2));
    if(leading < empties-leading) leading+=1;
    while(leading>0)
    {
      setChar(_firstDigitIndex+jPos,_justificationBlank, false);
      jPos++;
      empties--;
      leading--;
    }
    
    // and then the content
    for(uint8_t x = 0;x <= currentPos; x++)
    {
      setChar(_firstDigitIndex+jPos+x, *(justifyBuffer+x)&(~0b10000000), *(justifyBuffer+x)&(0b10000000));
    }
    
    // and then the rest of the empties
    jPos += currentPos+1;
    while(empties>0)
    {
      setChar(_firstDigitIndex+jPos,_justificationBlank, false);
      jPos++;
      empties--;
    }
  }
  else
  {
    // If we have run off the end of the (sub)display, do not actually print
    //  anything, just pretend we did
    if(currentPos < _numberOfDigits)
    {
      setChar( _firstDigitIndex + currentPos, c, dp);
    }
  }
}

LedPrintJustifiable LedPrintJustifiable::getSubDisplay(uint8_t firstDigitIndex, uint8_t numberOfDigits)
{
  return LedPrintJustifiable(_lc, firstDigitIndex, numberOfDigits, _displayIsBackwards, _justification);
}
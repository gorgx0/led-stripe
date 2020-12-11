#ifndef LedPrintJustifiable_h
#define LedPrintJustifiable_h

  #include "./LedPrint.h"
  
  /** Provides for right and center justification printing.
   * 
   * This is done as a separate class because it does incur a fair amount more memory
   * so if you don't need justification, then just use LedPrint
   * but if you do need justification, use LedPrintJustifiable
   * 
   */
  
  class LedPrintJustifiable : public LedPrint
  {
    friend class LedPrint;
    
    protected:
      /** For other-than-left justification, we need to keep a buffer
        *   of the current display, this is only allocated if you use 
        *   justify()
        */
      
      uint8_t     *justifyBuffer = NULL;
      int8_t      _justification = -1;
      char        _justificationBlank = ' ';
      
      /** Instantiate an LedPrint with an existing LedControl interface.  */
      
      LedPrintJustifiable ( LedControl *existingControl, uint8_t firstDigitIndex, uint8_t numberOfDigits, uint8_t displayIsBackwards, int8_t Justification = -1);

              
      /** Print a character at a (relative to (sub)display) position, with/withough dp 
        *  
        * Note that the position may be modified if using it would cause out-of-bounds
        * for the display.  This is the case for LedPrintJustifiable, while LedPrint 
        * currently just leaves the position as is but doesn't attempt to actually write.
        */
      
      void _printCharacter(uint8_t &currentPos, uint8_t c, uint8_t dp);
      
    public:
      
       LedPrintJustifiable() { };
       
       /** Get a subdisplay from an existing ledPrint so you can write to part of it
         *  useful if you have 2 or 3 seven-segment displays being run from a single
         *  MAX7219, for example you might have two 3 digit displays and use one MAX7219
         *  to drive them both.
         * 
         *   LedPrint myLedDisplay     = LedPrint(10,11,12, 6); // 6 digits total
         *   void setup()
         *   {
         *     LedPrint theFirstDisplay  = myLedDisplay.getSubDisplay(0,3); // First position is 0, 3 digits long
         *     LedPrint theSecondDisplay = myLedDisplay.getSubDisplay(3,3); // First position is 3, 3 digits long
         *   }
         */
       
       LedPrintJustifiable getSubDisplay(uint8_t firstDigitIndex, uint8_t numberOfDigits);
       
       /** Instantiate an LedPrint with the given dataPin, clkPin, csPin and an optional 
         *   number of digits (0...64)
         * 
         *      LedPrint myLedDisplay = LedPrint(10,11,12, 3); // a 3 digit display
         */
        
        LedPrintJustifiable (uint8_t dataPin, uint8_t clkPin, uint8_t csPin, uint8_t numDigits=8, uint8_t displayIsBackwards = 1) :
          LedPrint( dataPin,  clkPin,  csPin,  numDigits, displayIsBackwards) { };
        
      /** Set justification to left (-1), center (0), or right (1) 
        * 
        *  Setting to center or right will require more memory.
        * 
        *  Setting justification only affects subsequent printing.
        * 
        *  @param   Justification Integer of -1 for left, 0 for center or 1 for right
        *  @param blank The character to use to fill in blank spaces, eg '0' or ' ' 
        * 
        *  @return  The justification achieved, which might be -1 if no memory could be
        *            allocated to handle the other justification.
        */
      
      int8_t justify(int8_t Justification = 0, char blank = ' ');
      
      
  };
  
#endif
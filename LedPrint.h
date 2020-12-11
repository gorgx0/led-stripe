#ifndef LedPrint_h
#define LedPrint_h

  #include "./LedControl.h"
  
  // Forward declaration of this
  class LedPrintJustifiable;
  
  class LedPrint : public Print
  {
      protected:
        uint8_t     _displayIsBackwards = 0;
        
        uint8_t     _firstDigitIndex = 0;
        uint8_t     _numberOfDigits  = 8;
        
        uint8_t     currentPos = 0;
        uint8_t     lastByte   = 0;
        uint8_t     lastWasDP  = 0;

        LedControl *_lc;        
        
        /** Instantiate an LedPrint with an existing LedControl interface.  */
        LedPrint ( LedControl *existingControl, uint8_t firstDigitIndex, uint8_t numberOfDigits, uint8_t displayIsBackwards);
        
        /** Print a character at a (relative to (sub)display) position, with/withough dp 
         *  
         * Note that the position may be modified if using it would cause out-of-bounds
         * for the display.  This is the case for LedPrintJustifiable, while LedPrint 
         * currently just leaves the position as is but doesn't attempt to actually write.
         */
        
        virtual void _printCharacter(uint8_t &currentPos, uint8_t c, uint8_t dp);
        
        /** Set a given digit to a given character, with or without the decimal point.
         *  
         * The digit number must alread have been corrected for offset (+_firstDigitIndex)
         * in other words this is the absolute digit from the 0th digit of the LedControl
         */
        
        void setChar(uint8_t digit, char value, boolean dp)
        {
          if(_displayIsBackwards)
          {
            digit = (_lc->getDeviceCount()*8)-1-digit;
          }
          
          uint8_t addr = digit / 8;
          digit = digit - addr*8;
          _lc->setChar(addr, digit, value, dp);
        };
        
      public:
        
        LedPrint() { };
        
        /** Instantiate an LedPrint with the given dataPin, clkPin, csPin and an optional 
         *   number of digits (0...64)
         * 
         *      LedPrint myLedDisplay = LedPrint(10,11,12, 3); // a 3 digit display
         */
        
        LedPrint (uint8_t dataPin, uint8_t clkPin, uint8_t csPin, uint8_t numDigits=8, uint8_t displayIsBackwards = 1);        
        
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
        
        LedPrint          getSubDisplay(uint8_t firstDigitIndex, uint8_t numberOfDigits);
        
        /** As for getSubDisplay but returns a justifiable display (this is always the case when getting a 
         *   subdisplay from an already justifiable one, but for a non-justifiable parent display we 
         *   have this method to get a justifable child display).
         */
        
        LedPrintJustifiable getJustifiedSubDisplay(uint8_t firstDigitIndex, uint8_t numberOfDigits);
        
        /** Clear the display / sub-display
         */
        
        void clear();
        
        
        /** This is used by the print functions, you can use all print functions more or less.
         * 
         *     myLedDisplay.print(123.456);
         *     myLedDisplay.print("Hello");
         *     myledDisplay.print('a');
         * 
         *  Using println() will cause the following print command to clear thr display first.
         * 
         *     myLedDisplay.println(123); 
         *     delay(500);
         *     myLedDisplay.println(456);
         * 
         */
        
        size_t write(uint8_t c);
        
        
        /**
         * Set the power-down status of the led drivers, this operates globally for all devices
         * in the chain.
         * 
         * @param status True to shutdown, false to powerup
         */
        
        void shutdown(bool status = true)
        {
          for (uint8_t x = 0; x < _lc->getDeviceCount(); x++) { _lc->shutdown(x, status); }
        }
        
        /**
         * Set the intensity of the led drivers, this operates globally for all devices
         * in the chain.
         * 
         * @param intensity Integer value from 0 to 15
         */
        
        void setIntensity(uint8_t intensity) 
        { 
          for (uint8_t x = 0; x < _lc->getDeviceCount(); x++) { _lc->setIntensity(x, intensity); } 
        };
        
        /**
         * Clear all displays
         * 
         * @param intensity Integer value from 0 to 15
         */
        
        void clearEntireDisplay() 
        { 
          for (uint8_t x = 0; x < _lc->getDeviceCount(); x++) { _lc->clearDisplay(x); } 
        };
        

  };
  
  #include "./LedPrintJustifiable.h"
  
#endif
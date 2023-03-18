// You need external resistors on the LEDs
// Want to have a common cathode/common anode agnostic API

class RGBLED {
    public:
        RGBLED(int pinRed, int pinGreen, int pinBlue, bool isGndShared);
    
        void switchRedOn();
        void switchBlueOn();
        void switchGreenOn();
        void switchOff();
        void mix(int red, int green, int blue);


    private:
        void setCurrentColour(int red, int green, int blue);

    private:

        int m_pinRed;
        int m_pinGreen;
        int m_pinBlue;
        bool m_isCommonGnd;
        bool m_onState;
        bool m_offState;
        
        // R G B
        int m_currentColour[3];


};
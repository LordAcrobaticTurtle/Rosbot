#pragma once


class EncoderInterface {
    public:    
        virtual float readRPM() = 0;
        virtual long int readPosition () = 0;
        virtual void update () = 0;

};
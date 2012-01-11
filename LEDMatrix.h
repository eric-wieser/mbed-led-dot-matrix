#include "mbed.h"

class LEDMatrix
{
    public:
        LEDMatrix(BusOut* rows, BusOut* cols, char width, char height);
    private:
        void _drawFunc();
        char** _data;
        BusOut _rows;
        BusOut _cols;
        Ticker _redraw;
        float _refresh;

        char _width;
        char _height;

        char _bytesPerCol;

};
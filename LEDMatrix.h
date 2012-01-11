#include "mbed.h"
#include "bitarray.h"

class LEDMatrix
{
    public:
        LEDMatrix(char width, char height, float refresh);
        virtual void firstCol() = 0;
        virtual void nextCol() = 0;
        virtual void writeCol(BitArray data) = 0;
        virtual void clearCol();
    private:
        void _drawFunc();
        BitArray* _cols;
        Ticker _redraw;
        float _refresh;

        char _width;
        char _height;

        int _currentCol;

};
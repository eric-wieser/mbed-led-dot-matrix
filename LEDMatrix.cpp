#include "LEDMatrix.h"

LEDMatrix::LEDMatrix(BusOut* rows, BusOut* cols, char width, char height, float refresh)
: _rows(rows), _cols(cols), _refresh(refresh), _drawing(false), _scanmode(SCAN_ROWS) {
	_bytesPerCol = (height + 7) / 8;

	_data = new char*[width];
	for(int i = 0; i < width; i++) {
		_data[i] = new char[_bytesPerCol];
	}
}

void LEDMatrix::contains(int x, int y) {
	return 0 <= x && x < width && 0 <= y && y < width;
}

/**********************************************************/
/*Pixel operations                                        */
/**********************************************************/
void LEDMatrix::setPixel(int x, int y, int on)
{
	if(!contains(x, y)) return;
	on = on & 1;

	char &byte = _data[x][y / 8];
	char bit = y % 8;

	char mask = 1<<bit;
	byte = (byte & ~mask)|(on & mask);
}

int LEDMatrix::getPixel(int x, int y)
{
	if(!contains(x, y)) return 0;

	char &byte = _data[x][y / 8];
	char bit = y % 8;

	return (byte>>bit)&1;
}

/**********************************************************/
/*Row operations                                          */
/**********************************************************/
void LEDMatrix::setRow(int y, char* row)
{
	char byte = y / 8;
	char bit = y % 8;

	for (int x = 0; x < _width; x++)
	{
		_data[x][byte]
	}
	_data[row] = data;
}
void LEDMatrix::setCol(int y, char* row)
{
	char byte = y / 8;
	char bit = y % 8;

	for (int x = 0; x < _width; x++)
	{
		_data[x][byte]
	}
	_data[row] = data;
}

/**********************************************************/
/*Matrix operations                                       */
/**********************************************************/
void LEDMatrix::setData(int data[][8])
{
	for (int row = 0; row < 8; row++)
	{ 
		setRow(row,data[row]);
	}
	_curpos = 0;
}

void LEDMatrix::setData(char data[])
{
	for (int row = 0; row < 8; row++)
	{ 
		setRow(row,data[row]);
	}
}


/**********************************************************/
/*Drawing operations                                        */
/**********************************************************/
void LEDMatrix::_drawFunc()
{
	switch(_scanmode)
	{
	case SCAN_ROWS:
		_cols = 0xFF;
		_rows = (1<<_curpos);
		_cols = ~_data[_curpos];
		_curpos = (_curpos+1)%8;
		break;
	case SCAN_COLS:
		_rows = 0xFF;
		_cols = (1<<_curpos);
		char tempdata = 0;
		for(int row = 0; row<8; row++)
		{
			char val = getPixel(row,_curpos); 
			tempdata |= val<<row;
		}
		_rows = ~tempdata;
		_curpos = (_curpos+1)%8;
		break;
	}
}

void LEDMatrix::startDraw()
{
	if(!_drawing) _redraw.attach<LEDMatrix>(this, &LEDMatrix::_drawFunc,_refresh);
}

void LEDMatrix::stopDraw()
{
	if(_drawing) _redraw.detach();
}

/**********************************************************/
/*Other                                                   */
/**********************************************************/
void LEDMatrix::setScanMode(int mode)
{
	_scanmode = mode;
}


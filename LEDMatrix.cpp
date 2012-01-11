#include "LEDMatrix.h"

LEDMatrix::LEDMatrix(BusOut* rows, BusOut* cols, char width, char height, float refresh)
: _rows(rows), _cols(cols), _refresh(refresh), _drawing(false), _scanmode(SCAN_ROWS) {
	_cols = new BitArray*[width];
	for(int i = 0; i < width; i++) {
		_data[i] = BitArray(height);
	}
}

void LEDMatrix::contains(int x, int y) {
	return 0 <= x && x < width && 0 <= y && y < width;
}

/**********************************************************/
/*Pixel operations                                        */
/**********************************************************/
void LEDMatrix::setPixel(int x, int y, int on) {
	if(!contains(x, y)) return;
	_cols[x][y] = on != 0;
}

bool LEDMatrix::getPixel(int x, int y) {
	if(!contains(x, y)) return 0;
	return _cols[x][y];
}

/**********************************************************/
/*Row operations                                          */
/**********************************************************/
void LEDMatrix::setRow(int y, const &BitArray row) {
	for (int x = 0, size = row.size(); x < size; x++) {
		_cols[x][y] = row[x];
	}
}
void LEDMatrix::setCol(int x, const &BitArray col) {
	_data[x] = col;
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
	if(!_drawing) _redraw.attach<LEDMatrix>(this, &LEDMatrix::_drawFunc, _refresh);
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


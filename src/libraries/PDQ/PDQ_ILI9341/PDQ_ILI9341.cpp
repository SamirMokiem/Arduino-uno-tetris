#include "PDQ_ILI9341.h"
#include <stdint.h>
#include <SPI.h>
#include "PDQ_FastPin.h"
#include "../PDQ_ILI9341_config.h"

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void PDQ_ILI9341::commandList(const uint8_t *addr)
{
    uint8_t	numCommands, numArgs;
    uint16_t ms;

    numCommands = pgm_read_byte(addr++);		// Number of commands to follow
    while (numCommands--)				// For each command...
    {
        writeCommand(pgm_read_byte(addr++));	// Read, issue command
        numArgs	= pgm_read_byte(addr++);	// Number of args to follow
        ms = numArgs & DELAY;			// If hibit set, delay follows args
        numArgs &= ~DELAY;			// Mask out delay bit
        while (numArgs--)			// For each argument...
        {
            writeData(pgm_read_byte(addr++)); // Read, issue argument
        }

        if (ms)
        {
            ms = pgm_read_byte(addr++);	// Read post-command delay time (ms)
            if(ms == 255)
                ms = 500;		// If 255, delay for 500 ms
            delay(ms);
        }
    }
}

void PDQ_ILI9341::begin(void)
{

    // set CS and RS pin directions to output
    FastPin<ILI9341_CS_PIN>::setOutput();
    FastPin<ILI9341_DC_PIN>::setOutput();
#if !defined(AVR_HARDWARE_SPI)
#if defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    USICR = (0<<USISIE)|(0<<USIOIE)|(0<<USIWM1)|(1<<USIWM0)|(0<<USICS1)|(1<<USICS0)|(1<<USICLK)|(0<<USITC);
#endif
#if defined (ILI9341_MISO_PIN)
    FastPin<ILI9341_MISO_PIN>::setInput();
#endif
    FastPin<ILI9341_MOSI_PIN>::setOutput();
    FastPin<ILI9341_SCLK_PIN>::setOutput();
    FastPin<ILI9341_MOSI_PIN>::lo();
    FastPin<ILI9341_SCLK_PIN>::lo();
#endif

    FastPin<ILI9341_CS_PIN>::hi();		// CS <= HIGH (deselected, so no spurious data)
    FastPin<ILI9341_DC_PIN>::hi();		// RS <= HIGH (default data byte)

#if defined(AVR_HARDWARE_SPI)
    SPI.begin();
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE0);
	SPI.setClockDivider(SPI_CLOCK_DIV2);	// 8 MHz (full! speed!) [1 byte every 18 cycles]
#endif

#if ILI9341_SAVE_SPI_SETTINGS && defined(AVR_HARDWARE_SPI)
    // save our SPI settings
	save_SPCR = SPCR;
	save_SPSR = SPSR & 0x01; // SPI2X mask
#endif
    spi_begin();

    // Initialization commands for ILI9341 screens
    static const uint8_t ILI9341_cmds[] PROGMEM =
                                                {
                                                        22,
                                                        ILI9341_SWRESET, DELAY,		// 1
                                                        5,
                                                        0xEF, 3,			// 2
                                                        0x03, 0x80, 0x02,
                                                        0xCF, 3,			// 3
                                                        0x00, 0xC1, 0x30,
                                                        0xED, 4,			// 4
                                                        0x64, 0x03, 0x12, 0x81,
                                                        0xE8, 3,			// 5
                                                        0x85, 0x00, 0x78,
                                                        0xCB, 5,			// 6
                                                        0x39, 0x2C, 0x00, 0x34, 0x02,
                                                        0xF7, 1,			// 7
                                                        0x20,
                                                        0xEA, 2,			// 8
                                                        0x00, 0x00,
                                                        ILI9341_PWCTR1, 1,		// 9 power control
                                                        0x23,				// VRH[5:0]
                                                        ILI9341_PWCTR2, 1,		// 10 power control
                                                        0x10,				// SAP[2:0];BT[3:0]
                                                        ILI9341_VMCTR1, 2,		// 11 VCM control
                                                        0x3e, 0x28,
                                                        ILI9341_VMCTR2, 1,		// 12 VCM control2
                                                        0x86,				// --
                                                        ILI9341_MADCTL, 1,		// 13
                                                        (ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR),
                                                        ILI9341_PIXFMT, 1,		// 14
                                                        0x55,
                                                        ILI9341_FRMCTR1, 2,		// 15
                                                        0x00, 0x18,
                                                        ILI9341_DFUNCTR, 3,		// 16
                                                        0x08, 0x82, 0x27,
                                                        0xF2, 1,			// 17 3Gamma Function Disable
                                                        0x00,
                                                        ILI9341_GAMMASET, 1,		// 18 Gamma curve selected
                                                        0x01,
                                                        ILI9341_GMCTRP1, 15,		// 19 Set Gamma
                                                        0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
                                                        ILI9341_GMCTRN1, 15,		// 20
                                                        0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
                                                        ILI9341_SLPOUT, DELAY,		// 21
                                                        120,
                                                        ILI9341_DISPON, 0,		// 22
                                                };

    commandList(ILI9341_cmds);

    spi_end();
}

void PDQ_ILI9341::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    spi_begin();

    setAddrWindow_(x0, y0, x1, y1);

    spi_end();
}

void PDQ_ILI9341::pushColor(uint16_t color)
{
    spi_begin();

    spiWrite16_preCmd(color);

    spi_end();
}

void PDQ_ILI9341::pushColor(uint16_t color, int count)
{
    spi_begin();

    spiWrite16(color, count);

    spi_end();
}

void PDQ_ILI9341::drawPixel(int x, int y, uint16_t color)
{
    if ((x < 0) ||(x >= _width) || (y < 0) || (y >= _height))
        return;

    spi_begin();

    setAddrWindow_(x, y, x, y);

    spiWrite16_preCmd(color);

    spi_end();
}

void PDQ_ILI9341::drawFastVLine(int x, int y, int h, uint16_t color)
{
    // clipping
    if ((x < 0) || (x >= _width) || (y >= _height))
        return;

    if (y < 0)
    {
        h += y;
        y = 0;
    }

    int y1 = y+h;

    if (y1 < 0)
        return;

    if (y1 > _height)
        h = _height-y;

    spi_begin();

    setAddrWindow_(x, y, x, _height);
    spiWrite16(color, h);

    spi_end();
}


void PDQ_ILI9341::drawFastHLine(int x, int y, int w, uint16_t color)
{
    // clipping
    if ((x >= _width) || (y < 0) || (y >= _height))
        return;

    if (x < 0)
    {
        w += x;
        x = 0;
    }

    int x1 = x+w;

    if (x1 < 0)
        return;

    if (x1 > _width)
        w = _width-w;

    spi_begin();

    setAddrWindow_(x, y, _width, y);
    spiWrite16(color, w);

    spi_end();
}

void PDQ_ILI9341::fillRect(int x, int y, int w, int h, uint16_t color)
{
    // rudimentary clipping (drawChar w/big text requires this)
    if ((x >= _width) || (y >= _height))
        return;
    if (x < 0)
    {
        w += x;
        x = 0;
    }
    if (y < 0)
    {
        h += y;
        y = 0;
    }
    if ((x + w) > _width)
        w = _width  - x;
    if ((y + h) > _height)
        h = _height - y;

    spi_begin();

    setAddrWindow_(x, y, x+w-1, _height);

    for (; h > 0; h--)
    {
        spiWrite16(color, w);
    }

    spi_end();
}

// Bresenham's algorithm - thx Wikipedia
void PDQ_ILI9341::drawLine(int x0, int y0, int x1, int y1, uint16_t color)
{
#if 0 && defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny45__)
    drawLine_(x0, y0, x1, y1, color);
#else
    int8_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep)
    {
        swapValue(x0, y0);
        swapValue(x1, y1);
    }

    if (x0 > x1)
    {
        swapValue(x0, x1);
        swapValue(y0, y1);
    }

    if (x1 < 0)
        return;

    int dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int err = dx / 2;
    int8_t ystep;

    if (y0 < y1)
    {
        ystep = 1;
    }
    else
    {
        ystep = -1;
    }

    uint8_t setaddr = 1;

#if 0 && defined(__AVR_ATtiny85__) && !defined(__AVR_ATtiny45__)
    int	end = steep ? _height-1 : _width-1;
	if (x1 > end)
		x1 = end;

	for (; x0 <= x1; x0++)
	{
		if ((x0 >= 0) && (y0 >= 0) && (y0 <= end))
			break;

		err -= dy;
		if (err < 0)
		{
			err += dx;
			y0 += ystep;
		}
	}

	if (x0 > x1)
		return;

	spi_begin();

	for (; x0 <= x1; x0++)
	{
		if (setaddr)
		{
			if (steep)
				setAddrWindow_(y0, x0, y0, end+1);
			else
				setAddrWindow_(x0, y0, end+1, y0);
			setaddr = 0;
		}
		spiWrite16_lineDraw(color);
		err -= dy;
		if (err < 0)
		{
			y0 += ystep;
			if ((y0 < 0) || (y0 > end))
				break;
			err += dx;
			setaddr = 1;
		}
	}
#else
    if (steep)	// y increments every iteration (y0 is x-axis, and x0 is y-axis)
    {
        if (x1 >= _height)
            x1 = _height - 1;

        for (; x0 <= x1; x0++)
        {
            if ((x0 >= 0) && (y0 >= 0) && (y0 < _width))
                break;

            err -= dy;
            if (err < 0)
            {
                err += dx;
                y0 += ystep;
            }
        }

        if (x0 > x1)
            return;

        spi_begin();

        for (; x0 <= x1; x0++)
        {
            if (setaddr)
            {
                setAddrWindow_(y0, x0, y0, _height);
                setaddr = 0;
            }
            spiWrite16_lineDraw(color);
            err -= dy;
            if (err < 0)
            {
                y0 += ystep;
                if ((y0 < 0) || (y0 >= _width))
                    break;
                err += dx;
                setaddr = 1;
            }
#if defined(AVR_HARDWARE_SPI)
            else
			{
				__asm__ __volatile__
				(
					"	call	_ZN11PDQ_ILI93417delay10Ev\n"
					: : :
				);
			}
#endif
        }
    }
    else	// x increments every iteration (x0 is x-axis, and y0 is y-axis)
    {
        if (x1 >= _width)
            x1 = _width - 1;

        for (; x0 <= x1; x0++)
        {
            if ((x0 >= 0) && (y0 >= 0) && (y0 < _height))
                break;

            err -= dy;
            if (err < 0)
            {
                err += dx;
                y0 += ystep;
            }
        }

        if (x0 > x1)
            return;

        spi_begin();

        for (; x0 <= x1; x0++)
        {
            if (setaddr)
            {
                setAddrWindow_(x0, y0, _width, y0);
                setaddr = 0;
            }
            spiWrite16_lineDraw(color);
            err -= dy;
            if (err < 0)
            {
                y0 += ystep;
                if ((y0 < 0) || (y0 >= _height))
                    break;
                err += dx;
                setaddr = 1;
            }
#if defined(AVR_HARDWARE_SPI)
            else
			{
				__asm__ __volatile__
				(
					"	call	_ZN11PDQ_ILI93417delay10Ev\n"
					: : :
				);
			}
#endif
        }
    }
#endif

    spi_end();
#endif
}

void PDQ_ILI9341::setRotation(uint8_t m)
{
    rotation = (m & 3); // can't be higher than 3

    spi_begin();

    writeCommand(ILI9341_MADCTL);

    switch (rotation)
    {
        default:
        case 0:
            writeData(ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR);
            _width	= ILI9341_TFTWIDTH;
            _height = ILI9341_TFTHEIGHT;
            break;
        case 1:
            writeData(ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
            _width	= ILI9341_TFTHEIGHT;
            _height = ILI9341_TFTWIDTH;
            break;
        case 2:
            writeData(ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
            _width	= ILI9341_TFTWIDTH;
            _height = ILI9341_TFTHEIGHT;
            break;
        case 3:
            writeData(ILI9341_MADCTL_MV | ILI9341_MADCTL_MY | ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR);
            _width	= ILI9341_TFTHEIGHT;
            _height = ILI9341_TFTWIDTH;
            break;
    }

    spi_end();
}

void PDQ_ILI9341::invertDisplay(boolean i)
{
    spi_begin();

    writeCommand(i ? ILI9341_INVON : ILI9341_INVOFF);

    spi_end();
}

// Constructor when using hardware SPI.
PDQ_ILI9341::PDQ_ILI9341() : PDQ_GFX<PDQ_ILI9341>(ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT)
{
#if defined(AVR_HARDWARE_SPI)
    // must reference these functions from C++ or they will be stripped by linker (called from inline asm)
    delay10();
    delay13();
    delay15();
    delay17();
#endif
}
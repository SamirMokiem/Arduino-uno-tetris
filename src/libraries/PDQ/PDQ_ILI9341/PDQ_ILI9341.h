// This is the PDQ re-mixed version of Adafruit's library
// here is the original copyright notice:

/***************************************************
  This is an Arduino Library for the Adafruit 2.2" SPI display.
  This library works with the Adafruit 2.2" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/1480
 
  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

//===============================================================
// This PDQ optimized version is by Xark 
//
// Inspiration from Paul Stoffregen and the Teensy 3.1 community.
//
// GOALS:
//  1) Maintain "sketch" compatibility with original Adafruit libraries.
//  2) Be as much faster as is reasonably possible honoring goal 1. :-)
//  3) Be at least as small as Adafruit libraries.
//
// I believe all three of these have largely been achieved:
// 1) Near full compatibility.  Only minor initialization changes in original sketch.
// 2) Between ~2.5 and ~12 times faster (fillRect ~2.5x, drawLine ~12x).
// An average of ~4x faster over entire "graphictest.ino" benchmark.
//
// Even if this library is faster, it was based on the Adafruit original. 
// Adafruit deserves your support for making their library open-source (and
// for having some nice LCD modules and all kinds of other great parts too).
// Consider giving them your support if possible!

#if !defined(_PDQ_ILI9341H_)
#define _PDQ_ILI9341H_

#include <Arduino.h>
#include <Print.h>

#include "../PDQ_GFX/PDQ_GFX.h"
#include "../PDQ_ILI9341_config.h"

// #include "../../../../../../../../../../opt/homebrew/Cellar/avr-gcc@9/9.4.0/avr/include/avr/pgmspace.h"

#if !defined(ILI9341_CS_PIN) || !defined(ILI9341_DC_PIN)
#error Oops!  You need to #include "PDQ_ILI9341_config.h" (modified with your pin configuration and options) from your sketch before #include "PDQ_ILI9341.h".
#endif
#if defined(ILI9341_SAVE_SPCR)
#warning ILI9341_SAVE_SPCR is deprecated, use ILI9341_SAVE_SPI_SETTINGS
#define ILI9341_SAVE_SPI_SETTINGS ILI9341_SAVE_SPCR
#endif

#include "PDQ_FastPin.h"
#include "../PDQ_ILI9341_config.h"

#if !defined(__AVR_ATtiny85__) && !defined(__AVR_ATtiny45__)
#define INLINE		inline
#define INLINE_OPT	__attribute__((always_inline))
#else
#define INLINE
#define INLINE_OPT
#endif

// Color definitions
enum
{
	ILI9341_BLACK	= 0x0000,
	ILI9341_BLUE	= 0x001F,
	ILI9341_RED	= 0xF800,
	ILI9341_GREEN	= 0x07E0,
	ILI9341_CYAN	= 0x07FF,
	ILI9341_MAGENTA = 0xF81F,
	ILI9341_YELLOW	= 0xFFE0,	
	ILI9341_WHITE	= 0xFFFF,
};

class PDQ_ILI9341 : public PDQ_GFX<PDQ_ILI9341>
{
 public:
	// ILI9341 commands
	// For datasheet see https://www.adafruit.com/products/1480
 	enum
	{
		ILI9341_NOP			= 0x00,
		ILI9341_SWRESET		= 0x01,
		ILI9341_RDDID		= 0x04,
		ILI9341_RDDST		= 0x09,

		ILI9341_SLPIN		= 0x10,
		ILI9341_SLPOUT		= 0x11,
		ILI9341_PTLON		= 0x12,
		ILI9341_NORON		= 0x13,

		ILI9341_RDMODE		= 0x0A,
		ILI9341_RDMADCTL	= 0x0B,
		ILI9341_RDPIXFMT	= 0x0C,
		ILI9341_RDIMGFMT	= 0x0A,
		ILI9341_RDSELFDIAG	= 0x0F,

		ILI9341_INVOFF		= 0x20,
		ILI9341_INVON		= 0x21,
		ILI9341_GAMMASET	= 0x26,
		ILI9341_DISPOFF		= 0x28,
		ILI9341_DISPON		= 0x29,

		ILI9341_CASET		= 0x2A,
		ILI9341_PASET		= 0x2B,
		ILI9341_RAMWR		= 0x2C,
		ILI9341_RAMRD		= 0x2E,

		ILI9341_PTLAR		= 0x30,
		ILI9341_MADCTL		= 0x36,
		ILI9341_PIXFMT		= 0x3A,

		ILI9341_FRMCTR1		= 0xB1,
		ILI9341_FRMCTR2		= 0xB2,
		ILI9341_FRMCTR3		= 0xB3,
		ILI9341_INVCTR		= 0xB4,
		ILI9341_DFUNCTR		= 0xB6,

		ILI9341_PWCTR1		= 0xC0,
		ILI9341_PWCTR2		= 0xC1,
		ILI9341_PWCTR3		= 0xC2,
		ILI9341_PWCTR4		= 0xC3,
		ILI9341_PWCTR5		= 0xC4,
		ILI9341_VMCTR1		= 0xC5,
		ILI9341_VMCTR2		= 0xC7,

		ILI9341_RDID1		= 0xDA,
		ILI9341_RDID2		= 0xDB,
		ILI9341_RDID3		= 0xDC,
		ILI9341_RDID4		= 0xDD,

		ILI9341_GMCTRP1		= 0xE0,
		ILI9341_GMCTRN1		= 0xE1,

		// ILI9341_PWCTR6	= 0xFC,
	};

	// some other misc. constants
	enum
	{
		// screen dimensions
		ILI9341_TFTWIDTH	= 240,
		ILI9341_TFTHEIGHT	= 320,

		// MADCTL bits
		ILI9341_MADCTL_MH	= 0x04,	// bit 2 = 0 for refresh left -> right, 1 for refresh right -> left
		ILI9341_MADCTL_RGB	= 0x00,	// bit 3 = 0 for RGB color order
		ILI9341_MADCTL_BGR	= 0x08,	// bit 3 = 1 for BGR color order
		ILI9341_MADCTL_ML	= 0x10,	// bit 4 = 0 for refresh top -> bottom, 1 for bottom -> top
		ILI9341_MADCTL_MV	= 0x20,	// bit 5 = 0 for column, row order (portrait), 1 for row, column order (landscape)
		ILI9341_MADCTL_MX	= 0x40,	// bit 6 = 0 for left -> right, 1 for right -> left order
		ILI9341_MADCTL_MY	= 0x80,	// bit 7 = 0 for top -> bottom, 1 for bottom -> top

		// delay indicator bit for commandList()
		DELAY				= 0x80
	};

	// higher-level routines
	PDQ_ILI9341();
	static void begin(void);
	static void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	static void pushColor(uint16_t color);
	static void pushColor(uint16_t color, int cnt);

	// Pass 8-bit (each) R,G,B, get back 16-bit packed color
	static INLINE uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
	{
		return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
	}
	static INLINE uint16_t Color565(uint8_t r, uint8_t g, uint8_t b)	// older inconsistent name for compatibility
	{
		return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
	}
	
	// required driver primitive methods (all except drawPixel can call generic version in PDQ_GFX with "_" postfix).
	static void drawPixel(int x, int y, uint16_t color);
	static void drawFastVLine(int x, int y, int h, uint16_t color);
	static void drawFastHLine(int x, int y, int w, uint16_t color);
	static void setRotation(uint8_t r);
	static void invertDisplay(boolean i);

	static inline void fillScreen(uint16_t color) __attribute__((always_inline))
	{
		fillScreen_(color);			// call generic version
	}

	static void drawLine(int x0, int y0, int x1, int y1, uint16_t color);
	static void fillRect(int x, int y, int w, int h, uint16_t color);

	// === lower-level internal routines =========
	static void commandList(const uint8_t *addr);

	// NOTE: Make sure each spi_begin() is matched with a single spi_end() (and don't call either twice)
	// set CS back to low (LCD selected)
	static inline void spi_begin() __attribute__((always_inline))
	{
#if ILI9341_SAVE_SPI_SETTINGS && defined(AVR_HARDWARE_SPI)
		SPCR = save_SPCR;
		SPSR = save_SPSR;
#endif
		FastPin<ILI9341_CS_PIN>::lo();		// CS <= LOW (selected)
	}

	// NOTE: Make sure each spi_begin() is matched with a single spi_end() (and don't call either twice)
	// reset CS back to high (LCD unselected)
	static inline void spi_end() __attribute__((always_inline))
	{
		FastPin<ILI9341_CS_PIN>::hi();		// CS <= HIGH (deselected)
	}

#if defined(AVR_HARDWARE_SPI)
	// 10 cycle delay (including "call")
	static void delay10() __attribute__((noinline)) __attribute__((naked))
	{
		__asm__ __volatile__
		(
									// +4 (call to get here)
#if !defined(__AVR_HAVE_RAMPD__)	
			"	adiw	r24,0\n"	// +2 (2-cycle NOP)
#else
			"	nop\n"				// +1 (1-cycle NOP)
#endif
			"	ret\n"				// +4 (or +5 on >64KB AVR with RAMPD reg)
									// = 10 cycles
			: : : 
		);
	}

	// 13 cycle delay (including "call")
	static void delay13() __attribute__((noinline)) __attribute__((naked))
	{
		__asm__ __volatile__
		(
									// +4 (call to get here)
			"	adiw	r24,0\n"	// +2 (2-cycle NOP)
			"	adiw	r24,0\n"	// +2 (2-cycle NOP)
#if !defined(__AVR_HAVE_RAMPD__)	
			"	nop\n"				// +1 (1-cycle NOP)
#endif
			"	ret\n"				// +4 (or +5 on >64KB AVR with RAMPD reg)
									// = 13 cycles
			: : : 
		);
	}

	// 15 cycle delay (including "call")
	static void delay15() __attribute__((noinline)) __attribute__((naked))
	{
		__asm__ __volatile__
		(
									// +4 (call to get here)
			"	adiw	r24,0\n"	// +2 (2-cycle NOP)
			"	adiw	r24,0\n"	// +2 (2-cycle NOP)
			"	adiw	r24,0\n"	// +2 (2-cycle NOP)
#if !defined(__AVR_HAVE_RAMPD__)	
			"	nop\n"				// +1 (1-cycle NOP)
#endif
			"	ret\n"				// +4 (or +5 on >64KB AVR with RAMPD reg)
									// = 15 cycles
			: : : 
		);
	}

	// 17 cycle delay (including "call")
	static void delay17() __attribute__((noinline)) __attribute__((naked))
	{
		__asm__ __volatile__
		(
									// +4 (call to get here)
			"	adiw	r24,0\n"	// +2 (2-cycle NOP)
			"	adiw	r24,0\n"	// +2 (2-cycle NOP)
			"	adiw	r24,0\n"	// +2 (2-cycle NOP)
			"	adiw	r24,0\n"	// +2 (2-cycle NOP)
#if !defined(__AVR_HAVE_RAMPD__)	
			"	nop\n"				// +1 (2-cycle NOP)
#endif
			"	ret\n"				// +4 (or +5 on >64KB AVR with RAMPD reg)
									// = 17 cycles
			: : : 
		);
	}
	
	// normal SPI write with minimal hand-tuned delay (assuming max DIV2 SPI rate)
	static INLINE void spiWrite(uint8_t data) INLINE_OPT
	{
		SPDR = data;
		__asm__ __volatile__
		(
			"	call	_ZN11PDQ_ILI93417delay17Ev\n"	// call mangled delay17 (compiler would needlessly save/restore regs)
			: : : 
		);
	}

	// special SPI write with minimal hand-tuned delay (assuming max DIV2 SPI rate) - minus 2 cycles for RS (etc.) change
	static INLINE void spiWrite_preCmd(uint8_t data) INLINE_OPT
	{
		SPDR = data;

		__asm__ __volatile__
		(
			"	call	_ZN11PDQ_ILI93417delay15Ev\n"	// call mangled delay15 (compiler would needlessly save/restore regs)
			: : : 
		);
	}

	// SPI 16-bit write with minimal hand-tuned delay (assuming max DIV2 SPI rate)
	static INLINE void spiWrite16(uint16_t data) INLINE_OPT
	{
		uint8_t temp;
		__asm__ __volatile__
		(
			"	out	%[spi],%[hi]\n"				// write SPI data (18 cycles until next write)
			"	call	_ZN11PDQ_ILI93417delay17Ev\n"	// call mangled delay17 (compiler would needlessly save/restore regs)
			"	out	%[spi],%[lo]\n"				// write SPI data (18 cycles until next write)
			"	call	_ZN11PDQ_ILI93417delay17Ev\n"	// call mangled delay17 (compiler would needlessly save/restore regs)

			: [temp] "=d" (temp)
			: [spi] "i" (_SFR_IO_ADDR(SPDR)), [lo] "r" ((uint8_t)data), [hi] "r" ((uint8_t)(data>>8))
			: 
		);
	}

	// SPI 16-bit write with minimal hand-tuned delay (assuming max DIV2 SPI rate) minus 2 cycles
	static INLINE void spiWrite16_preCmd(uint16_t data) INLINE_OPT
	{
		uint8_t temp;
		__asm__ __volatile__
		(
			"	out	%[spi],%[hi]\n"				// write SPI data (18 cycles until next write)
			"	call	_ZN11PDQ_ILI93417delay17Ev\n"	// call mangled delay17 (compiler would needlessly save/restore regs)
			"	out	%[spi],%[lo]\n"				// write SPI data (18 cycles until next write)
			"	call	_ZN11PDQ_ILI93417delay15Ev\n"	// call mangled delay15 (compiler would needlessly save/restore regs)

			: [temp] "=d" (temp)
			: [spi] "i" (_SFR_IO_ADDR(SPDR)), [lo] "r" ((uint8_t)data), [hi] "r" ((uint8_t)(data>>8))
			: 
		);
	}

	// SPI 16-bit write with minimal hand-tuned delay (assuming max DIV2 SPI rate) minus 4 cycles
	static INLINE void spiWrite16_lineDraw(uint16_t data) INLINE_OPT
	{
		uint8_t temp;
		__asm__ __volatile__
		(
			"	out	%[spi],%[hi]\n"				// write SPI data (18 cycles until next write)
			"	call	_ZN11PDQ_ILI93417delay17Ev\n"	// call mangled delay17 (compiler would needlessly save/restore regs)
			"	out	%[spi],%[lo]\n"				// write SPI data (18 cycles until next write)

			: [temp] "=d" (temp)
			: [spi] "i" (_SFR_IO_ADDR(SPDR)), [lo] "r" ((uint8_t)data), [hi] "r" ((uint8_t)(data>>8))
			: 
		);
	}

	// normal SPI write with minimal hand-tuned delay (assuming max DIV2 SPI rate)
	static INLINE void spiWrite16(uint16_t data, int count) INLINE_OPT
	{
		uint8_t temp;
		__asm__ __volatile__
		(
			"	sbiw	%[count],0\n"				// test count
			"	brmi	4f\n"					// if < 0 then done
			"	breq	4f\n"					// if == 0 then done
			"1:	out	%[spi],%[hi]\n"				// write SPI data (18 cycles until next write)
			"	call	_ZN11PDQ_ILI93417delay17Ev\n"	// call mangled delay17 (compiler would needlessly save/restore regs)
			"	out	%[spi],%[lo]\n"				// write SPI data (18 cycles until next write)
			"	call	_ZN11PDQ_ILI93417delay13Ev\n"	// call mangled delay13 (compiler would needlessly save/restore regs)
			"	sbiw	%[count],1\n"				// +2	decrement count
			"	brne	1b\n"					// +2/1	if != 0 then loop
										// = 13 + 2 + 2 (17 cycles)			
			"4:\n"

			: [temp] "=d" (temp), [count] "+w" (count)
			: [spi] "i" (_SFR_IO_ADDR(SPDR)), [lo] "r" ((uint8_t)data), [hi] "r" ((uint8_t)(data>>8))
			: 
		);
	}
	
#else	// bit-bang
#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny45__)
	// USI hardware assisted
	static void spiWrite(uint8_t data) __attribute__((noinline))
	{
		USIDR = data;
		__asm__ __volatile__
		(
			"	out %[spi],%[clkp0]\n"	// MSB
			"	out %[spi],%[clkp1]\n"
			"	out %[spi],%[clkp0]\n"
			"	out %[spi],%[clkp1]\n"
			"	out %[spi],%[clkp0]\n"
			"	out %[spi],%[clkp1]\n"
			"	out %[spi],%[clkp0]\n"
			"	out %[spi],%[clkp1]\n"
			"	out %[spi],%[clkp0]\n"
			"	out %[spi],%[clkp1]\n"
			"	out %[spi],%[clkp0]\n"
			"	out %[spi],%[clkp1]\n"
			"	out %[spi],%[clkp0]\n"
			"	out %[spi],%[clkp1]\n"
			"	out %[spi],%[clkp0]\n"	// LSB
			"	out %[spi],%[clkp1]\n"
			: 
			: [spi] "i" (_SFR_IO_ADDR(USICR)), [clkp0] "a" ((uint8_t)((1<<USIWM0)|(0<<USICS0)|(1<<USITC))), [clkp1] "a" ((uint8_t)((1<<USIWM0)|(0<<USICS0)|(1<<USITC)|(1<<USICLK)))
			:
		);
	}
	static void spiWrite16(uint16_t data) __attribute__((noinline))
	{
		USIDR = data>>8;
		__asm__ __volatile__
		(
			"	out %[spi],%[clkp0]\n"	// MSB
			"	out %[spi],%[clkp1]\n"
			"	out %[spi],%[clkp0]\n"
			"	out %[spi],%[clkp1]\n"
			"	out %[spi],%[clkp0]\n"
			"	out %[spi],%[clkp1]\n"
			"	out %[spi],%[clkp0]\n"
			"	out %[spi],%[clkp1]\n"
			"	out %[spi],%[clkp0]\n"
			"	out %[spi],%[clkp1]\n"
			"	out %[spi],%[clkp0]\n"
			"	out %[spi],%[clkp1]\n"
			"	out %[spi],%[clkp0]\n"
			"	out %[spi],%[clkp1]\n"
			"	out %[spi],%[clkp0]\n"	// LSB
			"	out %[spi],%[clkp1]\n"
			: 
			: [spi] "i" (_SFR_IO_ADDR(USICR)), [clkp0] "a" ((uint8_t)((1<<USIWM0)|(0<<USICS0)|(1<<USITC))), [clkp1] "a" ((uint8_t)((1<<USIWM0)|(0<<USICS0)|(1<<USITC)|(1<<USICLK)))
			:
		);

		USIDR = data&0xff;
		__asm__ __volatile__
		(
			"	out %[spi],%[clkp0]\n"	// MSB
			"	out %[spi],%[clkp1]\n"
			"	out %[spi],%[clkp0]\n"
			"	out %[spi],%[clkp1]\n"
			"	out %[spi],%[clkp0]\n"
			"	out %[spi],%[clkp1]\n"
			"	out %[spi],%[clkp0]\n"
			"	out %[spi],%[clkp1]\n"
			"	out %[spi],%[clkp0]\n"
			"	out %[spi],%[clkp1]\n"
			"	out %[spi],%[clkp0]\n"
			"	out %[spi],%[clkp1]\n"
			"	out %[spi],%[clkp0]\n"
			"	out %[spi],%[clkp1]\n"
			"	out %[spi],%[clkp0]\n"	// LSB
			"	out %[spi],%[clkp1]\n"
			: 
			: [spi] "i" (_SFR_IO_ADDR(USICR)), [clkp0] "a" ((uint8_t)((1<<USIWM0)|(0<<USICS0)|(1<<USITC))), [clkp1] "a" ((uint8_t)((1<<USIWM0)|(0<<USICS0)|(1<<USITC)|(1<<USICLK)))
			:
		);
	}
#else
	static void spiWrite(uint8_t data) __attribute__((noinline))
	{
		// Fast SPI bitbang swiped from LPD8806 library
		for(uint8_t bit = 0x80; bit; bit >>= 1)
		{
			if (data & bit)
				FastPin<ILI9341_MOSI_PIN>::hi();
			else
				FastPin<ILI9341_MOSI_PIN>::lo();

			FastPin<ILI9341_SCLK_PIN>::hi();
			FastPin<ILI9341_SCLK_PIN>::lo();
		}
	}
	static void spiWrite16(uint16_t data) __attribute__((noinline))
	{
		spiWrite(data >> 8);
		spiWrite(data & 0xff);
	}
#endif
	static INLINE void spiWrite_preCmd(uint8_t data) INLINE_OPT
	{
		spiWrite(data);
	}
	static INLINE void spiWrite16_preCmd(uint16_t data) INLINE_OPT
	{
		spiWrite16(data);
	}
	static INLINE void spiWrite16_lineDraw(uint16_t data) INLINE_OPT
	{
		spiWrite16(data);
	}
	static INLINE void spiWrite16(uint16_t data, int count) INLINE_OPT
	{
		while (count-- > 0)
			spiWrite16(data);
	}
	static inline void delay10()	{ }
	static inline void delay13()	{ }
	static inline void delay15()	{ }
	static inline void delay17()	{ }
#endif

	// write SPI byte with RS (aka D/C) pin set low to indicate a command byte (and then reset back to high when done)
	static INLINE void writeCommand(uint8_t data) INLINE_OPT
	{
		FastPin<ILI9341_DC_PIN>::lo();		// RS <= LOW indicate command byte
		spiWrite_preCmd(data);
		FastPin<ILI9341_DC_PIN>::hi();		// RS <= HIGH indicate data byte (always assumed left in data mode)
	}

	// write SPI byte with RS assumed low indicating a data byte
	static inline void writeData(uint8_t data) __attribute__((always_inline))
	{
		spiWrite(data);
	} 

	// internal version that does not spi_begin()/spi_end()
	static INLINE void setAddrWindow_(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) INLINE_OPT
	{
		writeCommand(ILI9341_CASET); 		// column address set
		spiWrite16(x0);				// XSTART
		spiWrite16_preCmd(x1);			// XEND
		writeCommand(ILI9341_PASET); 		// row address set
		spiWrite16(y0);				// YSTART
		spiWrite16_preCmd(y1);		 	// YEND
		writeCommand(ILI9341_RAMWR); 		// write to RAM
	}

#if ILI9341_SAVE_SPI_SETTINGS && defined(AVR_HARDWARE_SPI)
	// our SPI settings, set these registers in spi_begin
	static volatile uint8_t	save_SPCR;
	static volatile uint8_t save_SPSR;
#endif
};

typedef PDQ_GFX_Button_<PDQ_ILI9341>	PDQ_GFX_Button;

/***************************************************
  This is an Arduino Library for the Adafruit 2.2" SPI display.
  This library works with the Adafruit 2.2" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/1480
 
  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#if ILI9341_SAVE_SPI_SETTINGS && defined(AVR_HARDWARE_SPI)
// static data needed by base class
volatile uint8_t PDQ_ILI9341::save_SPCR;
volatile uint8_t PDQ_ILI9341::save_SPSR;
#endif



#endif		// !defined(_PDQ_ILI9341H_)

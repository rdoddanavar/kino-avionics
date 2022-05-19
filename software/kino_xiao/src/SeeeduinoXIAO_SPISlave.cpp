#include <Arduino.h>
#include "SeeeduinoXIAO_SPISlave.h"

void Seeeduino_spiSlave_init()
{

    // Assign our board pins to SERCOM0
    gpio_set_pin_function(SPI_MOSI_PIN, PINMUX_PA10C_SERCOM0_PAD2);
    gpio_set_pin_function(SPI_SCK_PIN, PINMUX_PA11C_SERCOM0_PAD3);
    gpio_set_pin_function(SPI_MISO_PIN, PINMUX_PA08C_SERCOM0_PAD0);
    gpio_set_pin_function(SPI_CS_PIN, PINMUX_PA09C_SERCOM0_PAD1);

    // Disable SPI 0
    SERCOM0->SPI.CTRLA.bit.ENABLE = 0;
    while (SERCOM0->SPI.SYNCBUSY.bit.ENABLE);

    // Reset SPI 0
    SERCOM0->SPI.CTRLA.bit.SWRST = 1;
    while (SERCOM0->SPI.CTRLA.bit.SWRST || SERCOM0->SPI.SYNCBUSY.bit.SWRST);

    // Setting up NVIC
    NVIC_EnableIRQ(SERCOM0_IRQn);
    NVIC_SetPriority(SERCOM0_IRQn, 2);

    // Setting Generic Clock Controller!!!!
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCM_SERCOM0_CORE) | // Generic Clock 0
                        GCLK_CLKCTRL_GEN_GCLK0 | // Generic Clock Generator 0 is the source
                        GCLK_CLKCTRL_CLKEN; // Enable Generic Clock Generator

    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY); // Wait for synchronisation

    // Set up SPI Control A Register
    SERCOM0->SPI.CTRLA.bit.DORD = 0;     // MSB first
    SERCOM0->SPI.CTRLA.bit.CPOL = 0;     // SCK is low when idle, leading edge is rising edge
    SERCOM0->SPI.CTRLA.bit.CPHA = 0;     // Data sampled on leading sck edge and changed on a trailing sck edge
    SERCOM0->SPI.CTRLA.bit.FORM = 0x0;   // Frame format = SPI
    SERCOM0->SPI.CTRLA.bit.DIPO = 2;     // DATA PAD0 MOSI is used as input (slave mode)
    SERCOM0->SPI.CTRLA.bit.DOPO = 1;     // DATA PAD3 MISO is used as output
    //SERCOM0->SPI.CTRLA.bit.DIPO = 0;   // DATA PAD0 MOSI is used as input (slave mode)
    //SERCOM0->SPI.CTRLA.bit.DOPO = 0x2; // DATA PAD3 MISO is used as output
    SERCOM0->SPI.CTRLA.bit.MODE = 0x2;   // SPI in Slave mode
    SERCOM0->SPI.CTRLA.bit.IBON = 0x1;   // Buffer Overflow notification
    SERCOM0->SPI.CTRLA.bit.RUNSTDBY = 1; // Wake on receiver complete

    // Set up SPI control B register
    //SERCOM0->SPI.CTRLB.bit.RXEN = 0x1;    // Enable Receiver
    SERCOM0->SPI.CTRLB.bit.SSDE = 0x1;      // Slave Selecte Detection Enabled
    SERCOM0->SPI.CTRLB.bit.CHSIZE = 0;      // Character size 8 Bit
    //SERCOM0->SPI.CTRLB.bit.PLOADEN = 0x1; // Enable Preload Data Register
    //while (SERCOM0->SPI.SYNCBUSY.bit.CTRLB);

    // Set up SPI interrupts
    SERCOM0->SPI.INTENSET.bit.SSL = 0x1;   // Enable Slave Select low interrupt
    SERCOM0->SPI.INTENSET.bit.RXC = 0x1;   // Receive complete interrupt
    SERCOM0->SPI.INTENSET.bit.TXC = 0x1;   // Receive complete interrupt
    SERCOM0->SPI.INTENSET.bit.ERROR = 0x1; // Receive complete interrupt
    SERCOM0->SPI.INTENSET.bit.DRE = 0x1;   // Data Register Empty interrupt

    // Init SPI CLK
    //SERCOM0->SPI.BAUD.reg = SERCOM_FREQ_REF / (2*4000000u)-1;

    // Enable SPI
    SERCOM0->SPI.CTRLA.bit.ENABLE = 1;
    while (SERCOM0->SPI.SYNCBUSY.bit.ENABLE);
    SERCOM0->SPI.CTRLB.bit.RXEN = 0x1;       // Enable Receiver, this is done here due to errate issue
    while (SERCOM0->SPI.SYNCBUSY.bit.CTRLB); // Wait until receiver is enabled

}

static inline void gpio_set_pin_function(const uint32_t gpio, const uint32_t function)
{
  
    uint8_t port = GPIO_PORT(gpio);
    uint8_t pin  = GPIO_PIN(gpio);

    if (function == GPIO_PIN_FUNCTION_OFF)
    {
        hri_port_write_PINCFG_PMUXEN_bit(PORT, port, pin, false);
    }
    else
    {
        
        hri_port_write_PINCFG_PMUXEN_bit(PORT, port, pin, true);

        if (pin & 1)
        {
            // Odd numbered pin
            hri_port_write_PMUX_PMUXO_bf(PORT, port, pin >> 1, function & 0xffff);
        }
        else
        {
            // Even numbered pin
            hri_port_write_PMUX_PMUXE_bf(PORT, port, pin >> 1, function & 0xffff);
        }
    }

}

static inline void hri_port_write_PINCFG_PMUXEN_bit(const void *const hw, uint8_t submodule_index, uint8_t index, bool value)
{
    
    uint8_t tmp;
    PORT_CRITICAL_SECTION_ENTER();
    tmp = ((Port *)hw)->Group[submodule_index].PINCFG[index].reg;
    tmp &= ~PORT_PINCFG_PMUXEN;
    tmp |= value << PORT_PINCFG_PMUXEN_Pos;
    ((Port *)hw)->Group[submodule_index].PINCFG[index].reg = tmp;
    PORT_CRITICAL_SECTION_LEAVE();

}

static inline void hri_port_write_PMUX_PMUXO_bf(const void *const hw, uint8_t submodule_index, uint8_t index, hri_port_pmux_reg_t data)
{
    
    uint8_t tmp;
    PORT_CRITICAL_SECTION_ENTER();
    tmp = ((Port *)hw)->Group[submodule_index].PMUX[index].reg;
    tmp &= ~PORT_PMUX_PMUXO_Msk;
    tmp |= PORT_PMUX_PMUXO(data);
    ((Port *)hw)->Group[submodule_index].PMUX[index].reg = tmp;
    PORT_CRITICAL_SECTION_LEAVE();

}

static inline void hri_port_write_PMUX_PMUXE_bf(const void *const hw, uint8_t submodule_index, uint8_t index, hri_port_pmux_reg_t data)
{
    
    uint8_t tmp;
    PORT_CRITICAL_SECTION_ENTER();
    tmp = ((Port *)hw)->Group[submodule_index].PMUX[index].reg;
    tmp &= ~PORT_PMUX_PMUXE_Msk;
    tmp |= PORT_PMUX_PMUXE(data);
    ((Port *)hw)->Group[submodule_index].PMUX[index].reg = tmp;
    PORT_CRITICAL_SECTION_LEAVE();
    
}
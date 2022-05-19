/*
 * Seeeduino XIAO Hardware SPI Slave
 * 
 * Thanks goes out to:  
 *  jeremyherbert - https://github.com/jeremyherbert/playful-turtle-xiao
 *  The atmel SAMD21 library
 *  and everyone at https://forum.arduino.cc/index.php?topic=360026.0
 *  
 * Notes:
 *  - We are NOT using the default SPI pins per the Seeeduino documentation - we are reassigning these pins to SERCOM0.
 *  - MOSI and MISO should be connected direct to the SPI master device - do not reverse/swap them. 
 *  - If you are only receiving/listening, then you can leave pin MISO pin 4 disconnected.
 *  - You can use Serial for USB and Serial1 for UART TX/RX
 * 
 * Wiring:
 * SPI_MOSI  - Board Pin: 2 - Connect to MOSI on master
 * SPI_SCK   - Board Pin: 3
 * SPI_MISO  - Board Pin: 4 - Connect to MISO on master
 * SPI_CS    - Board Pin: 5
*/

// DO NOT CHANGE PIN ASSIGMENTS UNLESS YOU KNOW WHAT YOU ARE DOING
// ---- Seeeduino XIAO SPI ----

// Board pin is the numbered pin on the Seeeduino XIAO board (connect wires to these pins)
#define SPI_MOSI_B_PIN  2   // Board Pin 
#define SPI_SCK_B_PIN   3   // Board Pin 
#define SPI_MISO_B_PIN  4   // Board Pin 
#define SPI_CS_B_PIN    5   // Board Pin 

// Chip pin is the GPIO pin on the SAMD21 Microcontroller chip (internal only, you can ignore these)
#define SPI_MOSI_PIN    10  // Chip Pin SERCOM PAD2
#define SPI_SCK_PIN     11  // Chip Pin SERCOM PAD3
#define SPI_MISO_PIN    8   // Chip Pin SERCOM PAD0 
#define SPI_CS_PIN      9   // Chip Pin SERCOM PAD1

// Macros for the pin and port group
#define GPIO_PIN(n) (((n)&0x1Fu) << 0)
#define GPIO_PORT(n) ((n) >> 5)
#define GPIO(port, pin) ((((port)&0x7u) << 5) + ((pin)&0x1Fu))
#define GPIO_PIN_FUNCTION_OFF 0xffffffff

// Port Critical Sections
#if defined(ENABLE_PORT_CRITICAL_SECTIONS)
    #define PORT_CRITICAL_SECTION_ENTER() CRITICAL_SECTION_ENTER()
    #define PORT_CRITICAL_SECTION_LEAVE() CRITICAL_SECTION_LEAVE()
#else
    #define PORT_CRITICAL_SECTION_ENTER()
    #define PORT_CRITICAL_SECTION_LEAVE()
#endif

// Variables
typedef uint8_t  hri_port_pmux_reg_t;

// Functions
void Seeeduino_spiSlave_init();
void SERCOM0_Handler();
static inline void gpio_set_pin_function(const uint32_t gpio, const uint32_t function);
static inline void hri_port_write_PINCFG_PMUXEN_bit(const void *const hw, uint8_t submodule_index, uint8_t index, bool value);
static inline void hri_port_write_PMUX_PMUXO_bf(const void *const hw, uint8_t submodule_index, uint8_t index, hri_port_pmux_reg_t data);
static inline void hri_port_write_PMUX_PMUXE_bf(const void *const hw, uint8_t submodule_index, uint8_t index, hri_port_pmux_reg_t data);
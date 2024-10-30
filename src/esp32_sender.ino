
/* Heltec Automation send communication test example
 *
 * Function:
 * 1. Send data from a esp32 device over hardware 
 *  
 * Description:
 * 
 * HelTec AutoMation, Chengdu, China
 * 成都惠利特自动化科技有限公司
 * www.heltec.org
 *
 * this project also realess in GitHub:
 * https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
 * */

#include "LoRaWan_APP.h"
#include "Arduino.h"






#include <Wire.h>  
#include "HT_SSD1306Wire.h"

#ifdef WIRELESS_STICK_V3
static SSD1306Wire  display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_64_32, RST_OLED); // addr , freq , i2c group , resolution , rst
#else
static SSD1306Wire  display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED); // addr , freq , i2c group , resolution , rst
#endif



#define RF_FREQUENCY                                915000000 // Hz

#define TX_OUTPUT_POWER                             5        // dBm

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false


#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 250 // Define the payload size here

char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

double txNumber;
unsigned long timeTaken;

bool lora_idle=true;

static RadioEvents_t RadioEvents;
void OnTxDone( void );
void OnTxTimeout( void );


const float SERIESRESISTOR = 10000.0;
const float NOMINAL_RESISTANCE = 10000.0;
const float NOMINAL_TEMPERATURE = 25.0;
const float BCOEFFICIENT = 3950.0;
const int ADC_MAX_VALUE = 4095;
const float ABSOLUTE_ZERO = 273.15;

float calculate_temperature(int adc_value) {
    // Prevent division by zero
    if (adc_value == 0 || adc_value >= ADC_MAX_VALUE) {
        return NAN; // Retorna 'NaN' se o valor for inválido
    }

    // Calcula a resistência da termistor
    float resistance = (float(adc_value) * SERIESRESISTOR) / (ADC_MAX_VALUE - adc_value);

    // Aplica a equação de Steinhart-Hart
    float steinhart = resistance / NOMINAL_RESISTANCE;    // (R/Ro)
    steinhart = log(steinhart);                           // ln(R/Ro)
    steinhart /= BCOEFFICIENT;                            // 1/B * ln(R/Ro)
    steinhart += 1.0 / (NOMINAL_TEMPERATURE + ABSOLUTE_ZERO);  // + (1/To)
    steinhart = 1.0 / steinhart;                          // Inverter
    steinhart -= ABSOLUTE_ZERO;                           // Converter para Celsius

    return steinhart;
}









void setup() {
    Serial.begin(115200);
    Mcu.begin(HELTEC_BOARD,SLOW_CLK_TPYE);

    txNumber=0;

    RadioEvents.TxDone = OnTxDone;
    RadioEvents.TxTimeout = OnTxTimeout;

    Radio.Init( &RadioEvents );
    Radio.SetChannel( RF_FREQUENCY );
    Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000 ); 
   display.init()  ; 
   }





void loop()
{
  if(lora_idle == true)
  {
    delay(1000);

    int adc_value = analogRead(19);
    float temperaturaLida = calculate_temperature(adc_value);
    dtostrf(temperaturaLida, 2, 2, txpacket);



    display.setLogBuffer(3, 30);
    display.clear();
    // Print to the screen
    display.printf("temperatura lida:\n %f\n", temperaturaLida);
    display.printf("adc value lida: %d\n", adc_value);
    Serial.println(adc_value);
    // Draw it to the internal screen buffer
    display.drawLogBuffer(0, 0);
    // Display it on the screen
    display.display();
    
    Serial.printf("\r\nsending packet \"%s\" , length %d\r\n", txpacket, strlen(txpacket));

    timeTaken = millis();
    Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out
    

    lora_idle = false;
  }
  Radio.IrqProcess( );
}

void OnTxDone( void )
{
  timeTaken = millis() - timeTaken;
  Serial.println("TX done......");
  lora_idle = true;
  Serial.print("Tempo total: ");
  Serial.print(timeTaken);
  Serial.println(" ms");
}

void OnTxTimeout( void )
{
  timeTaken = millis() - timeTaken;
    Radio.Sleep( );
    Serial.println("TX Timeout......");
    lora_idle = true;
    Serial.print("Tempo total: ");
    Serial.print(timeTaken);
    Serial.println(" ms");
}

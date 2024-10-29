import serial
from decimal import Decimal, getcontext
import time
from datetime import datetime

# Set the precision for decimal calculations
getcontext().prec = 50

# Constants
SERIESRESISTOR = Decimal('10000')
NOMINAL_RESISTANCE = Decimal('10000')
NOMINAL_TEMPERATURE = Decimal('25')
BCOEFFICIENT = Decimal('3950')
ADC_MAX_VALUE = Decimal('1023')
ABSOLUTE_ZERO = Decimal('273.15')

def calculate_temperature(adc_value):
    adc_value = Decimal(adc_value)
    # Prevent division by zero
    if adc_value == 0 or adc_value >= ADC_MAX_VALUE:
        return None
    # Calculate the resistance of the thermistor
    resistance = (adc_value * SERIESRESISTOR) / (ADC_MAX_VALUE - adc_value)
    # Apply the Steinhart-Hart equation
    steinhart = resistance / NOMINAL_RESISTANCE     # (R/Ro)
    steinhart = steinhart.ln()                      # ln(R/Ro)
    steinhart /= BCOEFFICIENT                       # 1/B * ln(R/Ro)
    steinhart += Decimal('1') / (NOMINAL_TEMPERATURE + ABSOLUTE_ZERO)  # + (1/To)
    steinhart = Decimal('1') / steinhart            # Invert
    steinhart -= ABSOLUTE_ZERO                      # Convert to Celsius
    return float(steinhart)

def main():
    # Open the serial port (adjust '/dev/ttyUSB0' to your port)
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    time.sleep(2)  # Wait for the serial connection to initialize

    with open('temperature_readings.csv', 'a') as file:
        while True:
            line = ser.readline().decode('utf-8').strip()
            if line:
                try:
                    # Read ADC value from serial
                    leitura = int(line)
                    temperature = calculate_temperature(leitura)
                    if temperature is not None:
                        # Write temperature to file
                        current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
                        file.write(f"{current_time},{leitura},{temperature},\n")
                        file.flush()  # Ensure data is written to file
                        print(f"Temperature: {temperature:.5f} °C")
                    else:
                        print("Invalid ADC value (out of range).")
                except ValueError:
                    # Handle non-integer readings
                    print(f"Invalid ADC value received: {line}")
            time.sleep(1)  # Delay between readings

if __name__ == "__main__":
    main()

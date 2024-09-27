byte NTCPin = A0;

void setup()
{
Serial.begin(9600);
}
void loop() {
  int ADCvalue = analogRead(NTCPin);
  Serial.println(ADCvalue);
  delay(1000);
}
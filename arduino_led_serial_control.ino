char ledState = '0'; 
void setup() {
  pinMode(13, OUTPUT); // LED on pin 13
  digitalWrite(13, LOW); // Ensure LED is off
  Serial.begin(9600);  // Start serial communication at 9600 baud
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read(); // Read the incoming command
    if (command == '1') {
      digitalWrite(13, HIGH); // Turn the LED on
      ledState = '1';
    } else if (command == '0') {
      digitalWrite(13, LOW);  // Turn the LED off
      ledState = '0';
    } else if (command == 't') {
      if (ledState == '0') {
        digitalWrite(13, HIGH);
        ledState = '1';
      } else {
        digitalWrite(13, LOW);
        ledState = '0';
      }
    } else if (command == 'r') {
      Serial.write(ledState); // Send the current LED state back
    }
  }
}

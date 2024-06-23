void setup() {
  pinMode(13, OUTPUT); // LED on pin 13
  Serial.begin(9600);  // Start serial communication at 9600 baud
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read(); // Read the incoming command
    if (command == '1') {
      digitalWrite(13, HIGH); // Turn the LED on
    } else if (command == '0') {
      digitalWrite(13, LOW);  // Turn the LED off
    }
  }
}

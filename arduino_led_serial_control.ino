#define LED_PIN 13

char ledState = '0'; // Initial state: LED off
unsigned long blinkInterval = 500; // Default blink interval in milliseconds
int blinkCount = 0; // Number of times to blink

void setup() {
  pinMode(LED_PIN, OUTPUT); // LED on pin 13
  digitalWrite(LED_PIN, LOW); // Ensure LED is off
  Serial.begin(9600);  // Start serial communication at 9600 baud
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n'); // Read the incoming command

    if (command.startsWith("on")) {
      digitalWrite(LED_PIN, HIGH); // Turn the LED on
      ledState = '1';
    } else if (command.startsWith("off")) {
      digitalWrite(LED_PIN, LOW);  // Turn the LED off
      ledState = '0';
    } else if (command.startsWith("toggle")) {
      if (ledState == '0') {
        digitalWrite(LED_PIN, HIGH);
        ledState = '1';
      } else {
        digitalWrite(LED_PIN, LOW);
        ledState = '0';
      }
    } else if (command.startsWith("read")) {
      Serial.write(ledState); // Send the current LED state back
    } else if (command.startsWith("blink")) {
      blinkCount = command.substring(6).toInt(); // Extract blink count
    } else if (command.startsWith("set_interval")) {
      blinkInterval = command.substring(13).toInt(); // Extract interval
    } else if (command.startsWith("get_interval")) {
      Serial.println(blinkInterval); // Send the current interval back
    }
  }

  if (blinkCount > 0) {
    digitalWrite(LED_PIN, HIGH);
    delay(blinkInterval);
    digitalWrite(LED_PIN, LOW);
    delay(blinkInterval);
    blinkCount--;
  }
}

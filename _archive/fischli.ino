// INIT GLOBALS
// PINS
const int R = 0;
const int O = 1;
const int G = 2;
const int durationRedInput = A0;  // poti pin
const int durationGreenInput = A1;  // poti pin

// TIMING
const int maxDurationSeconds = 20*60; // Max seconds delay red/green
int durationRed = maxDurationSeconds;  // Seconds red light is on, changed by poti
int durationGreen = maxDurationSeconds;  // Seconds green light is on, changed by poti
const int phaseDuration = 4000;  // delay ms inbetween changing lights

long step = 0;  // iterator


// SETUP
void setup() {
  // initialize serial communication at 9600 bits per second
  Serial.begin(9600);
  pinMode(R, OUTPUT);
  pinMode(O, OUTPUT);
  pinMode(G, OUTPUT);

  pinMode(durationRedInput, INPUT);
  pinMode(durationGreenInput, INPUT);

  // Test colors and stay on red
  digitalWrite(R, HIGH);
  delay(phaseDuration);
  Serial.println("BEGIN");
  Serial.println("⚫️ ⚫️ 🔴");
  turnGreen();
  delay(phaseDuration/4);
  turnRed();
  Serial.println("LOOP");
}


// LOOPY LOOP
void loop() {
  // Update current delay red value
  durationRed = readPoti(durationRedInput);
  // Serial.println(durationRed);

  // Show progress
  Serial.print("⚫️ ⚫️ 🔴\t");
  Serial.print(step);
  Serial.print("\t");
  Serial.println(durationRed);

  // Turn green if red delay is over
  if(step >= durationRed) {
    step = 0;  // Reset for delay on green

    turnGreen();

    while(step < durationGreen) {
      // Show progress
      Serial.print("🟢 ⚫️ ⚫️\t");
      Serial.print(step);
      Serial.print("\t");
      Serial.println(durationGreen);

      // Update current delay green value
      durationGreen = readPoti(durationGreenInput);
      // Serial.println(durationGreen);

      // Increment steps for delay green
      step++;
      delay(1000);
    }

    step = 0;  // Reset for delay on red
    
    // Turn red if green delay is over
    turnRed();
  }

  // Increment steps for delay red
  step++;
  delay(1000);
}


// VAR FUNCTIONS
void turnRed() {
  Serial.println("⚫️ 🟠 ⚫️");
  digitalWrite(G, LOW);
  digitalWrite(O, HIGH);
  digitalWrite(R, LOW);
  delay(phaseDuration);

  Serial.println("⚫️ ⚫️ 🔴");
  digitalWrite(G, LOW);
  digitalWrite(O, LOW);
  digitalWrite(R, HIGH);
}

void turnGreen() {
  Serial.println("⚫️ 🟠 🔴");
  digitalWrite(G, LOW);
  digitalWrite(O, HIGH);
  digitalWrite(R, HIGH);
  delay(phaseDuration);

  Serial.println("🟢 ⚫️ ⚫️");
  digitalWrite(G, HIGH);
  digitalWrite(O, LOW);
  digitalWrite(R, LOW);
}

int readPoti(int pin) {
  // return maxDurationSeconds; // TESTING
  return map(analogRead(pin), 0, 1024, 1, maxDurationSeconds+1);
}
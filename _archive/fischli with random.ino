#include <Entropy.h>
#define randomSeed(s) srandom(s)

// INIT GLOBALS
// PINS
const int R = 0;
const int O = 1;
const int G = 2;
const int durationRedInput = 3;  // poti pin
const int durationGreenInput = 4;  // poti pin

// TIMING
// TODO
// maxDurationSteps controls step count of loops AND time of delay in each loop.
// Should be one or the other!
const int maxDurationSteps = 6; // in steps, if potis are all the way up, it equals to this
int durationRed = maxDurationSteps;  // Steps red light is on, changed by poti
int durationGreen = maxDurationSteps;  // Steps green light is on, changed by poti
const int phaseDuration = 4000;  // delay ms inbetween changing lights
int regularity = 100;  // 0 pure random (each step 0-1000ms), 100 = non-random (each step 1000ms)

const int minDurationSeconds = float(maxDurationSteps)/100*regularity;  // Minimum delay time for each step, <= maxDurationSteps
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

  // Seed the random() function with a false "true" random seed
  setupRandomness();

  // Test colors and stay on red
  delay(phaseDuration);
  Serial.println("BEGIN");
  digitalWrite(R, HIGH);
  Serial.println("⚫️ ⚫️ 🔴");
  delay(phaseDuration);
  turnGreen();
  delay(phaseDuration);
  turnRed();
}


// LOOPY LOOP
void loop() {
  // Update current delay red value
  durationRed = readPoti(durationRedInput);
  // Serial.println(durationRed);

  // Show progress
  Serial.println("⚫️ ⚫️ 🔴");

  // Turn green if red delay is over
  if(step >= durationRed) {
    step = 0;  // Reset for delay on green

    turnGreen();

    while(step < durationGreen) {
      // Show progress
      Serial.println("🟢 ⚫️ ⚫️");

      // Update current delay green value
      durationGreen = readPoti(durationGreenInput);
      // Serial.println(durationGreen);
      // delay(1000);
      waitForIT();
      // Increment steps for delay green
      step++;
    }


    step = 0;  // Reset for delay on red
    
    // Turn red if green delay is over
    turnRed();
  }

  // Increment steps for delay red
  step++;
  // delay(1000);
  waitForIT();
}

void waitForIT() {
  int randDelay = random(minDurationSeconds, maxDurationSteps+1)*1000;
  Serial.print("min\t");
  Serial.print(minDurationSeconds);
  Serial.print("\tmax\t");
  Serial.print(maxDurationSteps);
  Serial.print("\tdelay\t");
  Serial.println(randDelay);
  delay(randDelay);
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

int readPoti(int potiPin) {
  // TESTING
  return maxDurationSteps;
  // TODO uncomment
  return map(analogRead(potiPin), 0, 1024, 0, maxDurationSteps);
}

void setupRandomness() {
  uint32_t seed_value;
  Entropy.initialize();
  seed_value = Entropy.random();
  Serial.print("Seed value: ");
  Serial.println(seed_value);
  randomSeed(seed_value);  // Seed the seed
}
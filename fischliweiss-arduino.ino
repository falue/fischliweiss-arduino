// INIT GLOBALS
// PINS

/*
D2= Relay 4 = -- (empty)
D3= Relay 3 = GREEN
D4= Relay 2 = RED
D5= Relay 1 = ORANGE
*/
// RELAY PINS
const int R = 4;
const int O = 5;
const int G = 3;
const int clicker = 2;  // 4th unused relay: pin 2

const int durationRedInput = A0;  // poti pin
const int durationGreenInput = A2;  // poti pin A1 orig?
const int durationOrangeInput = A1;  // poti pin A2 orig?

// TIMING
const int maxDurationSeconds = 20*60;  // Max seconds delay red/green, ignoring runtime  // MAX 32760s = 546min = 9.1h
int durationRed = maxDurationSeconds;  // Seconds red light is on, changed by poti
int durationGreen = maxDurationSeconds;  // Seconds green light is on, changed by poti
const int maxDurationOrange = 20*60;  // Max seconds seconds delay orange
int durationOrange = maxDurationOrange;  // delay ms inbetween changing lights
int durationAudioClicker = 333;

long step = 0;  // iterator for green AND red

// SETUP
void setup() {
  delay(1500);
  // initialize serial communication at 9600 bits per second
  Serial.begin(9600);

  pinMode(R, OUTPUT);
  pinMode(O, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(clicker, OUTPUT);

  pinMode(durationRedInput, INPUT);
  pinMode(durationGreenInput, INPUT);
  pinMode(durationOrangeInput, INPUT);

  Serial.println("TRAFFIC LIGHT CONTROLLER");
  Serial.println("FOR PETER FISCHLI");
  Serial.println("programmed & engineered by fabian lüscher");
  Serial.println("@fabianluscher | www.fluescher.ch | info@fluescher.ch");
  Serial.println("v1.0 - Flashed @ 12.2.2022");

  // Test colors fast and stay on red
  startupTest();
  
  // Blink all lights three times to show its working
  blink(3);

  // Show who's boss
  click(4);

  // Keep red shining 
  digitalWrite(R, HIGH);

  Serial.println();
  Serial.println("LOOP BEGINS");
}


// LOOPY LOOP
void loop() {
  // Update current delay red value
  durationRed = readPoti(durationRedInput, maxDurationSeconds);
  // Serial.println(durationRed);

  // Show progress
  Serial.print("⚫️ ⚫️ 🔴\t");
  Serial.print(step);
  Serial.print("\t");
  Serial.println(durationRed);

  // Turn green if red delay is over
  if(step >= durationRed) {
    step = 0;  // Reset for delay on orange
    turnGreen();

    while(step < durationGreen) {
      // Show progress
      Serial.print("🟢 ⚫️ ⚫️\t");
      Serial.print(step);
      Serial.print("\t");
      Serial.println(durationGreen);

      // Update current delay green value
      durationGreen = readPoti(durationGreenInput, maxDurationSeconds);
      // Serial.println(durationGreen);

      // Increment steps for delay green
      step++;
      delay(1000);
    }

    step = 0;  // Reset for delay on orange
    
    // Turn red if green delay is over
    turnRed();
  }

  // Increment steps for delay red
  step++;
  delay(1000);
}


// VAR FUNCTIONS
void turnRed() {
  // Update Orange
  durationOrange = readPoti(durationOrangeInput, maxDurationOrange);

  Serial.print("⚫️ 🟠 ⚫️\t");
  digitalWrite(G, LOW);
  digitalWrite(O, HIGH);
  digitalWrite(R, LOW);
  click(1);
  Serial.println();

  step = 0;  // Reset for delay on green
  while(step < durationOrange) {
    // Show progress
    Serial.print("⚫️ 🟠 ⚫️\t");
    Serial.print(step);
    Serial.print("\t");
    Serial.println(durationOrange);

    // Update current delay green value
    durationOrange = readPoti(durationOrangeInput, maxDurationOrange);

    // Increment steps for delay green
    step++;
    delay(1000);
  }
  /* delay(durationOrange); */

  Serial.print("⚫️ ⚫️ 🔴\t");
  digitalWrite(G, LOW);
  digitalWrite(O, LOW);
  digitalWrite(R, HIGH);
  click(1);
  Serial.println("");

  step = 0;  // Reset for delay
}

void turnGreen() {
  // Update Orange
  durationOrange = readPoti(durationOrangeInput, maxDurationOrange);

  Serial.print("⚫️ 🟠 🔴\t");
  digitalWrite(G, LOW);
  digitalWrite(O, HIGH);
  digitalWrite(R, HIGH);
  click(1);
  Serial.println();

  step = 0;  // Reset for delay on green
  while(step < durationOrange) {
    // Show progress
    Serial.print("⚫️ 🟠 🔴\t");
    Serial.print(step);
    Serial.print("\t");
    Serial.println(durationOrange);

    // Update current delay green value
    durationOrange = readPoti(durationOrangeInput, maxDurationOrange);

    // Increment steps for delay green
    step++;
    delay(1000);
  }

  Serial.print("🟢 ⚫️ ⚫️\t");
  digitalWrite(G, HIGH);
  digitalWrite(O, LOW);
  digitalWrite(R, LOW);
  click(1);
  Serial.println("");

  step = 0;  // Reset for delay
}

int readPoti(int pin, int max) {
  return map(analogRead(pin), 0, 1024, 1, max+1);
}

void click(int times) {
  for(int i = 0; i < times; i++) {
    Serial.print("+ click! ");
    digitalWrite(clicker, HIGH);
    delay(durationAudioClicker);
    digitalWrite(clicker, LOW);
    if(times > 1) delay(durationAudioClicker);
  }
}



void startupTest() {
  // Do not use delay times defined by poti, so to see if everything works
  // without waiting 20mins or so
  Serial.println("startup test");

  // Start on Red
  Serial.println("⚫️ ⚫️ 🔴");
  digitalWrite(R, HIGH);
  delay(2000);

  // Go Orange & Red
  Serial.println("⚫️ 🟠 🔴");
  digitalWrite(G, LOW);
  digitalWrite(O, HIGH);
  digitalWrite(R, HIGH);
  delay(1000);

  // Go Green
  Serial.println("🟢 ⚫️ ⚫️");
  digitalWrite(G, HIGH);
  digitalWrite(O, LOW);
  digitalWrite(R, LOW);
  delay(2000);

  // Go Orange
  Serial.println("⚫️ 🟠 ⚫️");
  digitalWrite(G, LOW);
  digitalWrite(O, HIGH);
  digitalWrite(R, LOW);
  delay(1000);

  // Go Red
  Serial.println("⚫️ ⚫️ 🔴");
  digitalWrite(G, LOW);
  digitalWrite(O, LOW);
  digitalWrite(R, HIGH);
  delay(2000);
}

void blink(int times) {
  for(int i = 0; i < times; i++) {
    Serial.println("blink!");
    digitalWrite(R, HIGH);
    digitalWrite(O, HIGH);
    digitalWrite(G, HIGH);
    delay(666);
    digitalWrite(R, LOW);
    digitalWrite(O, LOW);
    digitalWrite(G, LOW);
    delay(666);
  }
}

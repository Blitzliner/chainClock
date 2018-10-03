#define PIN_STEP 2
#define PIN_DIR 3
#define PIN_BLINK 13
#define DELAY_STEP 10
#define DELAY_BIG 1000

#define STEPS_PER_REVOLUTION (63.68395 * 16.0)  // 32 for full step => 63.68395 * 32.0 = 2037.8864


int numSteps = STEPS_PER_REVOLUTION;//100;//100;//708;                 //720/5.625 degree = 128/2 = 64 //int numSteps = 48;Number of steps in 360 degree rotation
int rotations = 1;

void setup() {
  pinMode(PIN_STEP, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);
  pinMode(PIN_BLINK, OUTPUT);

  digitalWrite(PIN_DIR, HIGH);
}

void loop() {
  unsigned int idx;
  unsigned int totalSteps = (numSteps * rotations);
  
  digitalWrite(PIN_BLINK, HIGH);
  
  for (idx = 0; idx < totalSteps; ++idx) {
    digitalWrite(PIN_STEP, LOW);    // Prepare to take a step
    //delay(DELAY_STEP); /* not necessary but for debugging*/
    digitalWrite(PIN_STEP, HIGH) ;  // Take a step
    delay(DELAY_STEP);              // Allow some delay between energizing the coils to allow stepper rotor time to respond.
  }
  
  digitalWrite(PIN_BLINK, LOW);
  
  delay(DELAY_BIG);
}

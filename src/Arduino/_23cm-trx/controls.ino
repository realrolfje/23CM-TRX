/*
 * User inputs and outputs controlling the transceiver.
 */

/* Speed and direction of rotary turn */
volatile int rotary_turn = 0;
volatile unsigned long nextRotaryMs = 1;
 
void setupControls() {
  pinMode(TX_ON, OUTPUT);  
  pinMode(PTT, INPUT_PULLUP);

  pinMode(ROTARY_A,    INPUT_PULLUP);
  pinMode(ROTARY_B,    INPUT_PULLUP);
  pinMode(ROTARY_PUSH, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ROTARY_A), rotary_isr, FALLING); 
}


boolean isPTTPressed(){
  return !digitalRead(PTT);
}

/*
 * Returns the number of clicks the rotary encorder
 * turned clockwise since last read.
 * Negative clicks is counter clockwise.
 */
int getRotaryTurn() {
  int temp = constrain(rotary_turn, -10, 10);
  rotary_turn = 0;
  return temp;
}

/*
 * 0 - no push
 * 1 - short push
 * 2 - long push
 */
byte getRotaryPush() {
  if (digitalRead(ROTARY_PUSH)) {
    return 0;
  }
  
  unsigned long longPush = millis() + 500;
  while(!digitalRead(ROTARY_PUSH)){
    delay(10);
  };

  if (millis() > longPush) {
    return 2;
  } else {
    return 1;
  }
}

void rotary_isr() {
    unsigned long now = millis();
    if (now < nextRotaryMs) {
      return;
    }
    nextRotaryMs = now + 20; // debounce rotary milliseconds
  
    // ROTARY_A Gone low. check turn direction with ROTARY_B
    if (digitalRead(ROTARY_B)) {
      rotary_turn -= 1;
    } else {
      rotary_turn += 1;
    }
}



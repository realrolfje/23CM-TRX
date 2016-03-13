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

int getRotaryTurn() {
  int temp = rotary_turn;
  rotary_turn = 0;
  return temp;
}


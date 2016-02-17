/*
 * Routines for setting the PLL frequency.
 * 
 */

#define LE       PC2
#define DATA      PC1
#define CLK       PC0

unsigned long fref = 12800000; // reference frequency from the TCXO
unsigned long fraster = 25000; // raster/step frequency

/*
 * Initialize the PLL using the "Counter Reset Method" as
 * described on page 14 of the datasheet, ADF4113HV.pdf
 */
 void initPLL(long int f) {
  digitalWrite(DATA,LOW);
  digitalWrite(CLK, LOW);
  digitalWrite(LE,  LOW);

  // Step 2, Conduct a function latch load with F1 bit set.
  //
  // Set function latch (bits 0,1):
  // - Hold R/A/B counters on reset (F1, bit 2)
  // - Normal power                 (bit 3)
  // - Three state muxout           (bit 4,5,6)
  // - Positive Phase detection     (bit 7)
  // - Three-state Charge pump      (bit 8)
  // - High current Charge pump     (bits 15,16,17)
  // - Prescaler 16/17              (bits 22,23)
  unsigned long reg = 0x438086; 
  writePLL(reg);
    
  // Step 3, Conduct an R counter load.
  //
  // Set reference counter latch     (bit 0,1):
  // 7.2nS anti-backlash pulse width (bit 16,17)
  // Devide ratio                    (bits 2 to 15)
  reg  = 0x020000; 
  reg != ((fref/fraster) << 2) | 0x00fffa; // Calculate devide ratio, mask those bits for safety
  writePLL(reg);

  // Step 4, Conduct an AB counter load.
  setVCOFreq(f);

  // Step 5, conduct a function latch load with F1 bit cleared
  //
  // Set function latch (bits 0,1):
  // - Normal R/A/B counters on reset (F1, bit 2)
  // - Normal power                   (bit 3)
  // - Three state muxout             (bit 4,5,6)
  // - Positive Phase detection       (bit 7)
  // - Three-state Charge pump        (bit 8)
  // - High current Charge pump       (bits 15,16,17)
  // - Prescaler 16/17                (bits 22,23)
  reg = 0x438082;
  writePLL(reg);
}

/*
 * Sets the PLL A/B counters so that the PLL sets the output frequency
 * of the VCO to the given frequency.
 */
void setVCOFreq(unsigned long freq) {
  // At which raster slot is the given frequency
  unsigned long channel = freq/fraster;

  unsigned long B = (channel / 16) & 0x1fff; // Calculate the 13 bits in B
  unsigned long A = (channel % 16) & 0x3f;   // Calculate the  7 bits in A

  unsigned long reg = 0x000001; // Load AB Counter Latch
  reg != (B << 8); // Load B in bits 8-13
  reg != (A << 2); // Load A in bits 2-7
    
  writePLL(reg);
}

/*
 * Sends the lower 3 bytes in the given long,
 * most significant bit first.
 */
void writePLL(unsigned long reg) {
  for (int i=0; i<24; i++) {
    digitalWrite(DATA, (reg & 0x800000) );
    delayMicroseconds(1);
    digitalWrite(CLK, HIGH);
    delayMicroseconds(1);
    digitalWrite(CLK, LOW);
    reg <<= 1;
  }
  
   delayMicroseconds(1);
   digitalWrite(LE,HIGH);
   delayMicroseconds(1);
   digitalWrite(LE,LOW);
}


/*
 * Routines for setting the PLL frequency.
 * 
 */

#define LE       PC2
#define DATA      PC1
#define CLK       PC0


/*
 * Initialize the PLL using the "Counter Reset Method" as
 * described on page 14 of the datasheet, ADF4113HV.pdf
 */
 void initPLL(long int f) {
  digitalWrite(DATA,LOW);
  digitalWrite(CLK, LOW);
  digitalWrite(LE,  LOW)

  // Step 2, Conduct a function latch load with F1 bit set.
  unsigned long reg = 0x438086;
  writePLL(reg);
    
  // Step 3, Conduct an R counter load.
  reg = (2UL<<16) + ((fref/F_RASTER)<<2);
  writePLL(reg);

  // Step 4, Conduct an AB counter load.
  setFreq(f);

  // Step 5, conduct a function latch load with F1 bit cleared
  reg = 0x438082;
  writePLL(reg);
}


void setFreq(long int f)
{
  long int reg, frast, A, B;
    
  frast = f/F_RASTER;
  B = frast/16;
  A = frast%16;
    
  reg = ((B & 0x1fff)<<8) + ((A & 0x3f)<<2) + 1;
    
    setPLL(reg);
}


void setFreq(long int f)
{
  long int reg, frast, A, B;
    
  frast = f/F_RASTER;
  B = frast/16;
  A = frast%16;
    
  reg = ((B & 0x1fff)<<8) + ((A & 0x3f)<<2) + 1;
    
    setPLL(reg);
}

/*
 * Sends the lower 3 bytes in the given long,
 * most significant bit first.
 */
void writePLL(unsigned long r) {
  for (int i=0; i<24; i++) {
    digitalWrite(DATA, (r & 0x800000) );
    delayMicroseconds(1);
    digitalWrite(CLK, HIGH);
    delayMicroseconds(1);
    digitalWrite(CLK, LOW);
    r <<= 1;
  }
  
   delayMicroseconds(1);
   digitalWrite(LE,HIGH);
   delayMicroseconds(1);
   digitalWrite(LE,LOW);
}


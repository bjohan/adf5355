#define MUX  2
#define LE 3
#define DATA 11
#define CE 12
#define CLK 13



// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(MUX, INPUT);     
  pinMode(CE, INPUT); //has pullup on pll-board so make input for hiZ
  pinMode(DATA, OUTPUT);     
  pinMode(CLK, OUTPUT);     
  pinMode(LE, OUTPUT);     
}

void sendWord(uint32_t word)
{
	//PORTB &= ~_BV(PB3); //Clear LE
	digitalWrite(LE, LOW);
	//delay(10);
	for(uint32_t b = 32 ; b > 0 ; b--){
		//PORTB &= ~_BV(PB5); //Clear CLK
		digitalWrite(CLK, LOW);
		//delay(10);
		if(word & 0x80000000)
			digitalWrite(DATA, HIGH);
			//PORTB |= _BV(PB4); //Set data if a one should be sent
		else
			digitalWrite(DATA, LOW);
			//PORTB &= ~_BV(PB4); //Clear data if a zero should be sent
		//delay(10);
		//PORTB |= _BV(PB5); //Set CLK
		digitalWrite(CLK, HIGH);
		//delay(10);
		word <<= 1;
	}

		//delay(10);
	//PORTB |= _BV(PB3); //Set LE
	digitalWrite(LE, HIGH);
}

void sendRegister(uint32_t autocal, uint32_t prescaler, uint32_t intv){
	uint32_t word= 0;
	word |= (intv&0xFFFF)<<(4);
	word |= (prescaler&0x1)<<(4+16);
	word |= (autocal&0x1)<<(4+16+1);
	sendWord(word);
}

void sendRegister1(uint32_t frac1){
	sendWord(((frac1&0xFFFFFF)<<4) | 1);
}

void sendRegister2(uint32_t frac2, uint32_t mod2){
	uint32_t word = 2;
	word |= (mod2 & 0x3FFF) << 4;
	word |= (frac2 & 0x3FFF) << (4+14);
	sendWord(word);
}

void sendRegister3(uint32_t sd_load_reset, uint32_t phase_resync, uint32_t phase_adjust, uint32_t phase){
	uint32_t word = 3;
	word |= (phase & 0xFFFFFF) << 4;
	word |= (phase_adjust & 1) << (4+24);
	word |= (phase_resync & 1) << (4+24+1);
	word |= (sd_load_reset & 1) << (4+24+1+1);
	sendWord(word);
}

void sendRegister4(	uint32_t mux_out, 
			uint32_t reference_doubler, 
			uint32_t rdiv2, 
			uint32_t r_counter, 
			uint32_t double_buffered, 
			uint32_t current_setting, 
			uint32_t ref_mode, 
			uint32_t mux_logic, 
			uint32_t pd_polarity, 
			uint32_t pd, 
			uint32_t cp_tri_state, 
			uint32_t counter_reset){
	uint32_t word = 4;
	word |= (counter_reset & 0x01) << (4);
	word |= (cp_tri_state & 0x01) << (4+1);
	word |= (pd & 0x01) << (4+2);
	word |= (pd_polarity & 0x01) << (4+3);
	word |= (mux_logic&0x01) << (4+4);
	word |= (ref_mode&0x01) << (4+5);
	word |= (current_setting&0x04) << (4+6);
	word |= (double_buffered&0x01) << (4+6+4);
	word |= (r_counter&0x3FF) << (4+6+4+1);
	word |= (rdiv2&0x1) << (4+6+4+1+10);
	word |= (reference_doubler&0x1) << (4+6+4+1+10+1);
	word |= (mux_out&0x7) << (4+6+4+1+10+1+1);
	sendWord(word);
}


void loop() {
	delay(1000);
	sendRegister4(1,0,0,0,0,0,0,1,0,0,0,0);
	delay(1000);               // wait for a second
	sendRegister4(0,0,0,0,0,0,0,1,0,0,0,0);
}

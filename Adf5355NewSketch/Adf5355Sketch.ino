#define MUX  2
#define LE 3
#define DATA 11
#define CE 15
#define CLK 13
#define PFD_MAX 125000000
#define VCO_MIN 3400000000
#define VCO_MAX 6800000000
#define KHZ 1000
#define MHZ 1000*KHZ

#include <EEPROM.h>

bool macro=false;
uint8_t macroPosition=0;

void sendWord(uint32_t word)
{
	//Serial.print("Sending ");
	//Serial.print(word >> 4);
	//Serial.print(" to ");
	//Serial.println(word&0xF);
	digitalWrite(LE, LOW);
	for(uint32_t b = 32 ; b > 0 ; b--){
		digitalWrite(CLK, LOW);
		if(word & 0x80000000)
			digitalWrite(DATA, HIGH);
		else
			digitalWrite(DATA, LOW);
		digitalWrite(CLK, HIGH);
		word <<= 1;
	}
	digitalWrite(LE, HIGH);
}

void sendRegister0(uint32_t autocal, uint32_t prescaler, uint32_t intv){
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
	word |= (current_setting&0x0F) << (4+6);
	word |= (double_buffered&0x01) << (4+6+4);
	word |= (r_counter&0x3FF) << (4+6+4+1);
	word |= (rdiv2&0x1) << (4+6+4+1+10);
	word |= (reference_doubler&0x1) << (4+6+4+1+10+1);
	word |= (mux_out&0x7) << (4+6+4+1+10+1+1);
	sendWord(word);
}

void sendRegister5(){
	sendWord(0x00800025);
}

void sendRegister6(	uint32_t gated_bleed,
			uint32_t negative_bleed,
			uint32_t feedback_select,
			uint32_t rf_divider_select,
			uint32_t charge_pump_bleed_current,
			uint32_t mtld, 
			uint32_t rf_out_b,
			uint32_t rf_out_a,
			uint32_t rf_out_power){
	uint32_t word = 6;
	word |= (rf_out_power &0x03) << (4);
	word |= (rf_out_a& 0x01) << (4+2);
	word |= (0 &0x07) << (6+1); //Three reserved zeros
	word |= (rf_out_b &0x01) << (7+3);
	word |= (mtld &0x01) << (10+1);
	word |= (0 &0x01) << (11+1); //reserved zero
	word |= (charge_pump_bleed_current &0xFF) << (12+1);
	word |= (rf_divider_select&0x07) << (13+8);
	word |= (feedback_select &0x01) << (21+3);
	word |= (uint32_t)(0x1010 &0xF) << (24+1); //reserved 0xA
	word |= (negative_bleed &0x01) << (25+4);
	word |= (gated_bleed &0x01) << (29+1);
	sendWord(word);	
}

void sendRegister7(	uint32_t le_synchronization, 
			uint32_t ld_cycle_count,
			uint32_t lol_mode,
			uint32_t frac_n_ld_precision, 
			uint32_t ld_mode){
	uint32_t word = 7;
	word |= (ld_mode&0x1) << (4);
	word |= (frac_n_ld_precision&0x3) << (5);
	word |= (lol_mode&0x1) << (7);
	word |= (ld_cycle_count&0x3) << (8);
	word |= (le_synchronization&0x1) << (25);
	word |= (uint32_t)(0x04 &0x3F) << (26);
	sendWord(word);	
}

void sendRegister8(){
	sendWord(0x102D0428);
}

void sendRegister9(	uint32_t vco_band_division,
			uint32_t timeout,
			uint32_t automatic_level_timeout,
			uint32_t synthesizer_lock_timeout)
{
	uint32_t word = 9;
	word |= (synthesizer_lock_timeout&0x1F)<<(4);
	word |= (automatic_level_timeout&0x1F)<<(9);
	word |= (timeout&0x3FF)<<(14);
	word |= (vco_band_division&0xFF)<<(24);
	sendWord(word);	
}

void sendRegister10(uint32_t adc_clock_divider, uint32_t adc_conversion, uint32_t adc_enable){
	uint32_t word = 10;
	word |= (adc_enable &0x01) << 4;
	word |= (adc_conversion &0x01) << 5;
	word |= (adc_clock_divider &0xFF) << 6;
	word |= uint32_t( 0x00300&0x3FFFF) << 14;
	sendWord(word);	
}

void sendRegister11(){
	sendWord(0x0061300B);
}

void sendRegister12(uint32_t phase_resync_clock_divider){
	uint32_t word = 12;
	word |= (uint32_t)(0x41&0xFFF)<<4;
	word |= (phase_resync_clock_divider & 0xFFFF) << 16;
	sendWord(word);
}

uint64_t computePfd(uint64_t fref, uint32_t *ref_double, uint32_t *ref_counter, uint32_t *ref_div){
	//Computute a high PFD
	Serial.println("Computing PFD");
	*ref_double = 0;
	*ref_div = 0;
	if(fref < PFD_MAX){
		*ref_double = 1;
		fref=fref*2;
	}
	if(fref > PFD_MAX){
		*ref_counter = int((fref+PFD_MAX/2)/PFD_MAX);
		fref = fref/(*ref_counter);
	} else {
		*ref_counter = 1;
	}
	Serial.print("Ref freq: "); Serial.print((uint32_t)fref);
	Serial.print(" Ref doubler: "); Serial.print(*ref_double);
	Serial.print(" Ref halver: "); Serial.print(*ref_div);
	Serial.print(" Ref counter: "); Serial.println(*ref_counter);
	Serial.print("PFD frequency: "); Serial.println((uint32_t) fref);
	return fref;
}

uint64_t findVcoFrequency(uint64_t freq, uint32_t *rf_div){
	*rf_div = 0;
	Serial.println("Finding VCO frequency");
	if(freq > VCO_MAX){ 
		Serial.println("ERROR: Frequency above VCO range");
		Serial.println(freq/1000000.0);
		return 0;
	}
	for(int i = 0 ; i < 7; i++){
		Serial.println(freq/1000000.0);
		if(freq >= VCO_MIN && freq <= VCO_MAX){
			Serial.print("Found frequency: ");
			Serial.print(freq/1000000.0);
			Serial.print(" With divide by: ");
			Serial.println(1<<(*rf_div));
			return freq;
		}
		if(freq < VCO_MIN){
			freq = freq*2;
			*rf_div=*rf_div+1;
		}
	}
	Serial.println("ERROR: Frequency below VCO range");
	return 0;
}

uint32_t computeAdcClkDiv(uint64_t fpfd){
	uint32_t div;
	uint64_t q = (fpfd/100000)-2;
	div = (q+4)/4;
	div = div > 255 ? 255 : div;
	Serial.print("ADC_CLK_DIV: ") ; Serial.println(div);
	Serial.print("ADC rate: ") ; Serial.println(((uint32_t)fpfd)/div/4);
	return div;
}

uint32_t computeVcoBandDivision(uint64_t fpfd){
	uint32_t vcoBand = (fpfd+2399999)/2400000;
	Serial.print("VCO band division: "); Serial.println(vcoBand);
	return vcoBand;
} 

uint32_t computeInt(uint64_t fpfd, uint64_t fvco){
	uint32_t n = fvco/fpfd;
	float fVco = n*fpfd;
	Serial.print("INT is: "); Serial.println(n);
	Serial.print("VCO Frequency is: "); Serial.println(fVco/1000000.0);
	return n;
}

void setFrequencyIntMode(uint64_t frequency, uint64_t ref){
	uint32_t ref_doubler = 0;
	uint32_t ref_div = 0;
	uint32_t ref_counter = 0;
	uint64_t fpfd = 0;
	uint64_t fvco = 0;
	uint32_t rfdiv = 0;
	uint32_t adcDiv = 0;
	uint32_t vcoBandDiv = 0;
	uint32_t intn;
	uint32_t icp = 2;
	uint32_t bleedi = 0;
	fpfd = computePfd(ref, &ref_doubler, &ref_counter, &ref_div);
	fvco = findVcoFrequency(frequency, &rfdiv);
	adcDiv = computeAdcClkDiv(fpfd);
	vcoBandDiv = computeVcoBandDivision(fpfd);
	intn = computeInt(fpfd, fvco);
	//write registers
	sendRegister12(1); //Normal operation fpr phase resync
	sendRegister11();
	sendRegister10(adcDiv, 1, 1);
	sendRegister9(vcoBandDiv, 1023, 31, 31); //Timeouts set to max, fix!
	sendRegister8();
	sendRegister7(1, 3, 0, 3, 1); //Look at these values
	sendRegister6(0, 1, 1, rfdiv, bleedi, 1, 0, 1, 3); //Look in datasheet to compute bleed current
	sendRegister5();
	sendRegister4(6, ref_doubler, ref_div, ref_counter, 0, icp, 0, 1, 1, 0, 0, 0);
	sendRegister3(0, 0, 0, 0); //No phase resync
	sendRegister2(0,2); //No frac yet
	sendRegister1(0); //Also no frac
	delay(1);
	sendRegister0(1, 0, intn);
	
}



void setFrequencyFracMode(uint64_t frequency, uint64_t ref){
	uint32_t ref_doubler = 0;
	uint32_t ref_div = 0;
	uint32_t ref_counter = 0;
	uint64_t fpfd = 0;
	uint64_t fvco = 0;
	uint32_t rfdiv = 0;
	uint32_t adcDiv = 0;
	uint32_t vcoBandDiv = 0;
	uint32_t intn;
	uint32_t icp = 2;
	uint32_t bleedi = 0;
	uint32_t mod1 = 16777216;
	uint32_t diffFreq = 0;
	long double frac;
	uint32_t frac1;
	float frac2;
	uint32_t mod2=0x3FFF;
	float rfout;

	fpfd = computePfd(ref, &ref_doubler, &ref_counter, &ref_div);
	fvco = findVcoFrequency(frequency, &rfdiv);
	adcDiv = computeAdcClkDiv(fpfd);
	vcoBandDiv = computeVcoBandDivision(fpfd);

	intn = computeInt(fpfd, (float)fvco);
	diffFreq = ((fvco)- (intn*fpfd));///((double)(fpfd));
	Serial.print("DiffFreq: "); Serial.println(diffFreq);
	Serial.print("intn: "); Serial.println(intn);

	Serial.print("rfDiv: "); Serial.println(rfdiv);
	Serial.print("ref_doubler: "); Serial.println(ref_doubler);
	Serial.print("ref_div: "); Serial.println(ref_div);
	Serial.print("ref_counter: "); Serial.println(ref_counter);

	frac = ((double)diffFreq)/((double)fpfd);
	Serial.print("Frac: "); Serial.println(float(frac*10000000.0));

	frac1 = frac*mod1;
	Serial.print("Frac1: "); Serial.println(frac1);

	frac2 = frac*((double)mod1);//*((double)mod2);
	//frac2 = frac2&mod2;
	Serial.print("Frac2: "); Serial.println(frac2);
	//frac2 = 0;
	//mod2 = 2;
	
	rfout = (((float)intn)+( ((float)frac1)+( ((float)frac2)/((float)mod2)))/((float)mod1))*((float)fpfd)/((float)(1<<rfdiv));
	//rfout = 1e-6*(((double)intn))*((double)fpfd)/((double)(1<<rfdiv));


	Serial.print("Rf out: "); Serial.println((float) rfout/10.0);
	
	//write registers
	sendRegister12(1); //Normal operation fpr phase resync
	sendRegister11();
	sendRegister10(adcDiv, 1, 1);
	sendRegister9(vcoBandDiv, 1023, 31, 31); //Timeouts set to max, fix!
	sendRegister8();
	sendRegister7(1, 3, 0, 3, 1); //Look at these values
	sendRegister6(0, 0, 1, rfdiv, bleedi, 1, 0, 1, 3); //Look in datasheet to compute bleed current
	sendRegister5();
	sendRegister4(6, ref_doubler, ref_div, ref_counter, 0, icp, 0, 1, 1, 0, 0, 0);
	sendRegister3(0, 0, 0, 0); //No phase resync
	sendRegister2(frac2,mod2); //No frac yet
	sendRegister1(frac1); //Also no frac
	delay(1);
	sendRegister0(1, 0, intn);
	
}


void replayMacro(){
	uint8_t nm=EEPROM.read(0);
	Serial.print("Replaying macro consisting of ");
	Serial.print(nm);
	Serial.println(" writes");
	if(nm == 0xFF){
		Serial.println("Nonexistant macro, exiting");
		return;
	}
	for(int i = 0 ; i < nm ; i++){
		uint32_t sw;
		EEPROM.get(1+i*4, sw);
		Serial.print("Macro ");
		Serial.print(i);
		Serial.print(" value ");
		Serial.println(sw);
		sendWord(sw);
	}
}

// the setup routine runs once when you press reset:
void setup() {                
	// initialize the digital pin as an output.
	pinMode(MUX, INPUT);     
	pinMode(CE, INPUT); //has pullup on pll-board so make input for hiZ
	pinMode(DATA, OUTPUT);     
	pinMode(CLK, OUTPUT);     
	pinMode(LE, OUTPUT);
	Serial.begin(115200);
	//setFrequencyIntMode((uint64_t)6800*MHZ, (uint64_t)10*MHZ);
	//setFrequencyFracMode((uint64_t)1920*MHZ, (uint64_t)280*MHZ);
	//setFrequencyFracMode((uint64_t)880*MHZ, (uint64_t)280*MHZ);
	replayMacro();
	Serial.println("READY");

	
}




uint8_t copyTok(char *dst, char *src, int8_t n){
	for(;*src!='\0'; src++){
		if(*src == ' ') n--;
		else if(n == 0) *dst++=*src;
		if(n < 0 || (n==0 && *(src+1) =='\0')){
			*dst='\0'; 
			return 0;
		}
	}
	return -1;
}


void commandWriteRegister(char *cmdstr){
	uint8_t addr;
	uint32_t value;	
	char tokbuf[16];
	if(copyTok(tokbuf, cmdstr, 1)){
		Serial.println("First argument must be address");
	}
	addr=atoi(tokbuf);
	if(copyTok(tokbuf, cmdstr, 2)){
		Serial.println("Second argument must be value");
	}
	value=atol(tokbuf);
	/*Serial.println(cmdstr);
	Serial.print("Writing ");
	Serial.print(value);
	Serial.print(" to ");
	Serial.println(addr);*/
	uint32_t sw=(addr&0xF)|((value<<4)&0xFFFFFFF0);
	sendWord(sw);
	if(macro){
		EEPROM.put(4*macroPosition+1, sw);
		macroPosition++;
	}
}

void commandReadRegister(char *cmdstr){
	Serial.println("unimplemented");
}

void commandEnable(char *cmdstr){
	uint8_t enable;
	char tokbuf[16];
	if(copyTok(tokbuf, cmdstr, 1)){
		Serial.println("Firt argument must be 0 to disable or nonzero to enable");
	}
	enable=atoi(tokbuf);
	if(enable){
		pinMode(CE, INPUT);
		digitalWrite(CE, HIGH);
		Serial.println("Chip enabled");
	}else{
		pinMode(CE, OUTPUT);
		digitalWrite(CE, LOW);
		Serial.println("Chip disabled");
	}	
}

void commandMacroRecord(char *cmdstr){
	uint8_t enable;
	char tokbuf[16];
	if(copyTok(tokbuf, cmdstr, 1)){
		Serial.println("First argument must be 0 to finish recording or nonzero to start");
	}
	enable=atoi(tokbuf);
	if(enable){
		if(not macro){
			macro=true;
			macroPosition=0;
			Serial.println("Macro enabled");
		} else {
			Serial.println("Macro was already enabled");
		}
	}else{
		if(macro){
			macro=false;
			EEPROM.write(0, macroPosition);
			macroPosition=0;
			Serial.println("Macro disabled");
		} else {
			Serial.println("macro was already disabled");
		}
	}	

}

void commandReboot(char *cmdstr){
	Serial.println("toggling CE");
	pinMode(CE, OUTPUT);
	digitalWrite(CE, LOW);
	delay(100);
	digitalWrite(CE, HIGH);
	pinMode(CE, INPUT);
	replayMacro();

}

void commandOutput(char *cmdstr){
	Serial.print("PLL Output ");
	Serial.println(digitalRead(MUX));
}

void executeCommand(char* cmdstr){
	char tokbuf[16];
	if(copyTok(tokbuf, cmdstr, 0) == 0){
		switch(tokbuf[0]){
			case 'W':
			case 'w':
				commandWriteRegister(cmdstr);
				break;
			case 'R':
			case 'r':
				commandReadRegister(cmdstr);
				break;

			case 'E':
			case 'e':
				commandEnable(cmdstr);
				break;

			case 'M':
			case 'm':
				commandMacroRecord(cmdstr);
				break;

			case 'B':
			case 'b':
				commandReboot(cmdstr);
				break;

			case 'O':
			case 'o':
				commandOutput(cmdstr);
				break;
			default:
				Serial.print("Unrecognized command ");
				Serial.println(cmdstr);
			


		}
	}
	
}

void loop() {
	char d;
	static char cmd[16];
	static uint8_t cp=0;
	if(Serial.available()){
		d=Serial.read();
		if(d=='\r' ||  d== '\n'){
			cmd[cp]='\0';
			executeCommand(cmd);
			Serial.println("DONE");
			cp=0;
		} else {
			if(cp < 15){
				cmd[cp++]=d;
			} else {
				cmd[0]='\0';
				Serial.println("Command too long. Max 15 bytes. Press enter and enter new command");
			} 
		}
	}
}

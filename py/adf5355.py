#!/usr/bin/python3
import serial
import sys
import math
import mpmath as mp
import fractions
mp.mp.dps=100


class Adf5355:
    def __init__(self, port, timeout=5):
        if isinstance(port, str):
            self.d=serial.Serial(port, 115200, timeout=timeout);
        else:
            self.d=port

        print("Waiting for ADF5355 to boot");
        msg = self.readOutput(finalToken=b'READY');
        print("Booted successfully");
        #print("Boot message")
        #print(msg.decode('ascii'))

    def write(self, cmd):
        if len(cmd)>15:
            raise ValueError("Command is too long, 16 digits is the max");
        self.d.write(cmd+b'\n')

    def readOutput(self, finalToken=b'DONE\r\n'):
        o=b'';
        while True:
            d=self.d.read(1)
            if len(d) == 0:
                raise Exception("Did not get expected response before timeout. Expected to get %s in \"%s\""%(finalToken, o))
            o+=d;
            if finalToken in o:
                return o[0:-len(finalToken)];

    def query(self, cmd):
        self.write(cmd);
        return self.readOutput()

    def macro(self, state):
        if state:
            return self.query(b"M 1");
        else:
            return self.query(b"M 0");

    def writeRegister(self, addr, value):
        #print("Writing", value, "to", addr);
        cmdstr=b"W %d %d"%(addr, value)
        #print("command to send:", cmdstr)
        #print(self.query(cmdstr).decode('ascii'))
        self.query(cmdstr)


    def locked(self):
        d=self.query(b'O')
        d=d.strip()
        s=d.split(b"PLL Output")[-1].strip()
        return s==b'1'

    def createRegister0(self, autocal, prescaler, intValue):
        v='{0:010b}{1:01b}{2:01b}{3:016b}'.format(0,autocal, prescaler, intValue);
        #print("reg0", v, len(v))
        return int(v, 2)
        
    def createRegister1(self, frac1):
        v='{0:04b}{1:024b}'.format(0, frac1);
        #print("reg1", v, len(v))
        return int(v, 2)
        
    def createRegister2(self, frac2, mod2):
        v='{0:014b}{1:014b}'.format(frac2, mod2);
        #print("reg2", v, len(v))
        return int(v, 2)
        
    def createRegister3(self, sdLoad, phaseResync, phaseAdjust, phaseValue):
        v='{0:01b}{1:01b}{2:01b}{3:01b}{4:024b}'.format(False, sdLoad, phaseResync, phaseAdjust, phaseValue);
        #print("reg3", v, len(v))
        return int(v, 2)
        
    def createRegister4(self, muxOut, referenceDoubler, rdiv2, rCounter, doubleBuff, currentSetting, refMode, muxLogic, pdPolarity, pD, cpState, counterReset):
        #print(0, muxOut, referenceDoubler, rdiv2, rCounter, doubleBuff, currentSetting, refMode, muxLogic, pdPolarity, pD, cpState, counterReset);
        v='{0:02b}{1:03b}{2:01b}{3:01b}{4:010b}{5:01b}{6:04b}{7:01b}{8:01b}{9:01b}{10:01b}{11:01b}{12:01b}'.format(0, muxOut, referenceDoubler, rdiv2, rCounter, doubleBuff, currentSetting, refMode, muxLogic, pdPolarity, pD, cpState, counterReset);
        #print("reg4", v, len(v))
        return int(v, 2)
    
    def createRegister5(self):
        return int('0000000010000000000000000010', 2)

    def createRegister6(self, gateBleed, negativeBleed, feedbackSelect, rfDividerSelect, chargePumpBleedCurrent, mtld, rfOutB, rfOutA, rfOutputPower):
        v='{0:01b}{1:01b}{2:01b}{3:04b}{4:01b}{5:03b}{6:08b}{7:01b}{8:01b}{9:01b}{10:03b}{11:01b}{12:02b}'.format(False, gateBleed, negativeBleed, int('1010', 2), feedbackSelect, rfDividerSelect, chargePumpBleedCurrent, False, mtld, rfOutB, int('000', 2), rfOutA, rfOutputPower);
        #print("reg6", v, len(v))
        return int(v, 2)
    
    def createRegister7(self, leSync, ldCycleCount, lolMode, fracNlDPrecision, ldMode):
        v='{0:06b}{1:01b}{2:015b}{3:02b}{4:01b}{5:02b}{6:01b}'.format(int('000100', 2), leSync, 0, ldCycleCount, lolMode, fracNlDPrecision, ldMode);
        #print("reg7", v, len(v))
        return int(v, 2)
    
    def createRegister8(self):
        return 0x102D042 

    def createRegister9(self, vcoBandSelection, timeout, automaticLevelTimeout, synthesizerLockTimeout):
        v='{0:08b}{1:010b}{2:05b}{3:05b}'.format(vcoBandSelection, timeout, automaticLevelTimeout, synthesizerLockTimeout);
        #print("reg9", v, len(v))
        return int(v, 2)
    
    def createRegister10(self, adcClockDivider, adcConversion, adcEnable):
        v='{0:018b}{1:08b}{2:01b}{3:01b}'.format(int('000000001100000000',2), adcClockDivider, adcConversion, adcEnable);
        #print("reg10", v, len(v))
        return int(v, 2)
    
    def createRegister11(self):
        return int('0000000001100001001100000000', 2)
    
    def createRegister12(self, resyncClock):
        v='{0:016b}{1:012b}'.format(resyncClock, int('000001000001', 2));
        #print("reg12", v, len(v))
        return int(v, 2)
    
    def computeRdivDoublerAndFPfd(self, fref, maxFreq=125e6):
        doubler=1
        if fref < 100e6:
            doubler=2
        div=1
        for i in range(1023):
            if fref*doubler/div <= maxFreq:
                break;
            div+=1;
        if div%2==0 and doubler == 2:
            doubler = 1;
            div=div/2;
        return div, doubler==2, fref*doubler/div


    def findVcoFrequencyAndRfdiv(self, f):
        if f>6.8e9:
            raise ValueError("Frequency too high");
        div = 0
        while f < 3.4e9:
            div+=1;
            f*=2;
        if div > 6:
            raise ValueError("Frequency too low")
        return f, div


    def computeNFracAndMod1(self, fvco):

        ps = fractions.Fraction(4, 5);
        #limited by prescaler, currently set for 4/5 prescaler
        minimumMultiplicationFactor=23


        #compute phase detector settings
        maxFpfd = int(float(fvco)/float(minimumMultiplicationFactor)-0.5)
        if maxFpfd > 125e6:
            maxFpfd=125e6
        print("maximum reference", maxFpfd)

        rCounter, referenceDoubler, fpfd=self.computeRdivDoublerAndFPfd(self.fref, maxFpfd);
        dbl=1;
        if referenceDoubler:
            dbl=2;
        fpfd=fractions.Fraction(int(self.fref*dbl), int(rCounter))
        print("fpfd", fpfd, "rCounter", rCounter, "refdoubler", referenceDoubler)


        #compute values for intn, frac1 and mod1
        N=fractions.Fraction(int(fvco), fpfd)
        print("Requested multiplication factor", N)

        INT = int(N)
        T=(N-INT)*0x1000000
        print("T", T)
        FRAC1=int(T)
        print("FRAC1", FRAC1)

        RT = T-FRAC1
        print("RT", RT)
        FRAC2 = RT.numerator
        MOD2 = RT.denominator

        if MOD2 > 0x3FFF:
            sf=float(0x3FF)/MOD2
            MOD2=int(MOD2*sf)
            FRAC2=int(FRAC2*sf)
            print("Exact solution not possible, rescaling to approximate")

        
        print("INT", INT, "FRAC1", FRAC1, "FRAC2", FRAC2, "MOD2", MOD2)
        fvcoOut = (mp.mpf(fpfd.numerator)/mp.mpf(fpfd.denominator))*(mp.mpf(INT)+(mp.mpf(FRAC1)+mp.mpf(FRAC2)/mp.mpf(MOD2))/mp.mpf(0x1000000))
        print("Vco frequency will be", fvcoOut)
        return INT, FRAC1, FRAC2, MOD2, rCounter, referenceDoubler, fpfd, fvcoOut


    def initialize(self, frequency, fref=280e6):
        self.fref=fref
        rCounter, referenceDoubler, fpfd=self.computeRdivDoublerAndFPfd(fref);
        #print("fRef", fref, "rCounter", rCounter, "referenceDoubler", referenceDoubler, "fpd", fpfd)
        
        fvcoOpt, rfDividerSelect=self.findVcoFrequencyAndRfdiv(frequency);
        print("FvcoOpt", fvcoOpt, "rfDividerSelect", rfDividerSelect, "resulting in division of", 2**rfDividerSelect);

        INT, FRAC1, FRAC2, MOD2, rCounter, referenceDoubler, fpfd, fvco = self.computeNFracAndMod1(fvcoOpt)
        print("INT", INT, "FRAC1", FRAC1, "FRAC2", FRAC2, "MOD2", MOD2, "rcounter", rCounter, "reference doubler", referenceDoubler, "fpfd", fpfd)
        print("Output frequency", fvco/(2**rfDividerSelect))
        #nOpt=fvcoOpt/fpfd;
        #intValue=math.floor(nOpt);
        #print("nOpt", nOpt, "intValue", intValue);

        #fracPart=nOpt-intValue;
        #mod1=16777216;
        #frac1=int(fracPart*mod1)
        #nCurrent = intValue+frac1/mod1;

        #print("nCurrent", nCurrent, "ndiff", nOpt-nCurrent)
        #print("fvco", fpfd/nCurrent)

        adcClockDivider=math.ceil( ((fpfd/100000)-2)/4)
        if adcClockDivider > 255:
            adcClockDivider = 255;

        #print("ADC clock divider", adcClockDivider)
    
        vcoBandSelection=math.ceil(fpfd/2.4e6)
        #print("VCO band selection", vcoBandSelection)
        self.writeRegister(12, self.createRegister12(resyncClock=1))
        self.writeRegister(11, self.createRegister11())
        self.writeRegister(10, self.createRegister10(adcClockDivider=adcClockDivider, adcConversion=True, adcEnable=True))
        self.writeRegister(9, self.createRegister9(vcoBandSelection=vcoBandSelection, timeout=1023, automaticLevelTimeout=31, synthesizerLockTimeout=31))
        self.writeRegister(8, self.createRegister8())
        self.writeRegister(7, self.createRegister7(leSync=True, ldCycleCount=3, lolMode=False, fracNlDPrecision=3, ldMode=False)) #ldMode=False far frac-n operation
        self.writeRegister(6,  self.createRegister6(gateBleed=True, negativeBleed=True, feedbackSelect=True, rfDividerSelect=rfDividerSelect, chargePumpBleedCurrent=0, mtld=False, rfOutB=True, rfOutA=True, rfOutputPower=3)) #Check feedback select
        self.writeRegister(5, self.createRegister5())
        self.writeRegister(4, self.createRegister4(muxOut=6, referenceDoubler=referenceDoubler, rdiv2=False, rCounter=rCounter, doubleBuff=False, currentSetting=0, refMode=False, muxLogic=True, pdPolarity=True, pD=False, cpState=False, counterReset=False))
        self.writeRegister(3, self.createRegister3(sdLoad=False, phaseResync=False, phaseAdjust=False, phaseValue=0))
        self.writeRegister(2, self.createRegister2(frac2=FRAC2, mod2=MOD2))
        self.writeRegister(1, self.createRegister1(frac1=FRAC1))
        self.writeRegister(0, self.createRegister0(autocal=True, prescaler=0, intValue=INT))
    
if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(prog="adf5355.py", description="Program for controlling adf5355 pll", epilog="Have fun!")
    parser.add_argument("port", help='For instance /dev/ttyUSB0')
    parser.add_argument('-f', '--frequency', help="Set frequency", type=float);
    parser.add_argument('-r', '--reference', help="Reference frequency", type=float);
    parser.add_argument('-m', '--macro', help="Record macro while setting frequency that will be executed at boot. This means that the device will set the specified frequency during power on even without a computer", action='store_true')
    parser.add_argument('-l', '--locked', help="Check lock status", action='store_true');
    args=parser.parse_args()



    d=Adf5355(args.port)

    if len(sys.argv) < 2:
        print("Specify serial port to use");
        quit(0)

    reference=280e6;
    if args.reference:
        reference=args.reference
    if args.macro:
        print(d.macro(True).decode('ascii').strip())
    if args.frequency:
        d.initialize(args.frequency, fref=reference)
        #d.computeNFracAndMod1(args.frequency)
    if args.locked:
        import time
        time.sleep(0.1)
        print("Lock status is", d.locked())
    if args.macro:
        print(d.macro(False).decode('ascii').strip())
    input("press enter to continue");


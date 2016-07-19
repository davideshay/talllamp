#pragma SPARK_NO_PREPROCESSOR
// This #include statement was automatically added by the Particle IDE.
#include "PietteTech_DHT/PietteTech_DHT.h"

// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_TCS34725/Adafruit_TCS34725.h"

#include "FastLED/FastLED.h"
#include <vector>
#include <string>
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <stdlib.h>
#include "lampstructs.h"

FASTLED_USING_NAMESPACE;

// Paper Lamp with WS2812 Control Program
// David Shay, Georgia Shay, Rubie Shay
// 180 LEDs in 3 strips. First strip of 60, Second strip of 120 to limit data pins

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    2
#define DATA_PIN2   3
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    180
#define NUM_LEDS_STRIP1 60
#define NUM_LEDS_STRIP2 120

CRGB leds[NUM_LEDS];

#define BRIGHTNESS          180
#define FRAMES_PER_SECOND   60
#define FASTLED_NO_INTERRUPTS

#define SPOKELEN 60
CRGB spoke[SPOKELEN];
CRGB lamp[NUM_LEDS];

CRGB colorsinglecolor;
CRGB color3colors[3];

static CEveryNSeconds newPatternTime(5);
static CEveryNMillis newMinorEventMilli(20);
static CEveryNMillis newMajorEventMilli(100);
int secondsBetweenPatterns = 4;
uint16_t milliSecMinorEventDelay = 20;
uint16_t milliSecMajorEventDelay = 100;
const int defaultSecondsBetweenPatterns = 5;
std::string rainbowDirection = "F";
std::string switchMode = "A";
std::string curPattern;


uint8_t totalPatternCount;
uint8_t definedPatterns = 14;

uint8_t smoothburstarraysize;
uint8_t starburstarraysize;
uint8_t blendburstarraysize;
uint8_t fireworkcolorarraysize;
uint8_t fireworkcoloridx = 0;
uint8_t fireworkbands = 2;
uint8_t fireworklen = 10;
std::string fireworkExtraData;
uint8_t fireworkpixelidx = 0;
uint8_t cybigidx = 0;
uint8_t cyloncolorarraysize;
uint8_t cyloncoloridx = 0;
std::string  cylondir = "O";
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t smoothburstcoloridx=0;
uint8_t smoothburstpixelidx=0;
uint8_t starburstcoloridx = 0;
uint8_t starburstpixelidx = 0;
uint8_t blendburstcoloridx = 0;
uint8_t blendburstblendidx = 0;
uint8_t blendburstpixelidx = 0;
uint8_t fallingconfettirange = 15;
uint8_t fallingconfettiidx = SPOKELEN;
uint8_t multiconfetticoloridx=0;
uint8_t multiconfettiarraysize=0;
uint8_t cycleconfetticoloridx=0;
uint8_t cycleconfettiarraysize=0;
uint8_t cycleconfetticyclelength=50;
uint8_t cycleconfetticycleidx=0;
uint8_t curpatternlen = 60;
uint8_t smoothburstnumbands=5;
uint8_t starburstnumbands = 6;
uint8_t blendburstnumbands = 5;
uint8_t lightningFrequency = 50;
uint8_t lightningFlashes = 8;
uint8_t lightningDimmer = 1;
uint8_t growtimeidx = 0;
uint8_t growcolorarraysize = 0;
uint8_t growcoloridx = 0;
signed int growdirection = 1;
uint8_t firegravpixelidx =0;
bool firegravup = true;
uint8_t firegravcoloridx =0;
uint8_t firegravcolorarraysize = 0;
uint8_t rotatecoloridx = 0;
uint8_t rotateblendidx = 0;
uint8_t barbercoloridx = 0;
uint8_t barberblendidx = 0;
uint8_t barberstripewidth = 4;
uint8_t barberpixelidx = 0;
uint8_t firegravstopidx = 45;
CRGB raindropcolor = CRGB::Blue;
uint8_t cyclecolorcoloridx = 0;
uint8_t cyclecolorblendidx = 0;
uint8_t cyclecolorarraysize = 0;
uint8_t stormraintimeidx = 0;
uint8_t stormlightningtimeidx = 0;
uint8_t stormFrequency = 50;
uint8_t stormFlashes = 8;
uint8_t stormDimmer = 1;
std::vector<sParticle> firegravparticles;
std::vector<sRaindrop> rainDrops1;
std::vector<sRaindrop> rainDrops2;
std::vector<sRaindrop> rainDrops3;
std::vector<std::vector<sRaindrop>> rainDropSpokes = {rainDrops1,rainDrops2,rainDrops3};
std::vector<sRaindrop> stormDrops1;
std::vector<sRaindrop> stormDrops2;
std::vector<sRaindrop> stormDrops3;
uint8_t currentstormstart;
uint8_t currentstormend;
uint8_t stormnumflashes;
uint8_t stormflashidx = 0;
uint8_t stormendflashidx = random8(4,10);
uint8_t stormnextflashidx = 0;
std::vector<std::vector<sRaindrop>> stormDropSpokes = {stormDrops1, stormDrops2, stormDrops3};
CRGB stormrainLEDS[180];
CRGB stormlightningLEDS[180];
std::vector<sSplotch> colorSplotches;
float gravity = 0.05;
DirectionMode fireBurstMode;


bool processLoopEvents = true;


std::vector<sPattern> patternList;
std::vector<sPattern>::size_type patternIdx;
//std::vector<sPattern>::iterator patternIterator;
std::vector<sColor> colorList;
std::vector<sDuration> durationList;
std::vector<sSpeed> speedList;
std::vector<sExtra> extraList;
std::vector<CRGB> colorSubList;

std::vector<CRGB> cylonColorList;
std::vector<CRGB> starburstColorList;
std::vector<CRGB> fireworkColorList;
std::vector<CRGB> blendburstColorList;
std::vector<CRGB> multiconfettiColorList;
std::vector<CRGB> cycleconfettiColorList;
std::vector<CRGB> smoothburstColorList;
std::vector<CRGB> growColorList;
std::vector<CRGB> firegravColorList;
std::vector<CRGB> rotateColorList;
std::vector<CRGB> barberColorList;
std::vector<CRGB> cyclecolorColorList;

String debugbigmsg;
String debugvar;

CRGB myblend(float blendvalue, CRGB color1, CRGB color2);


// Log message to cloud, message is a printf-formatted string
void debug(String message, int value) {
    char msg [50];
    sprintf(msg, message.c_str(), value);
    Particle.publish("DEBUG", msg);
}

void debugbig(String message) {
    Particle.publish("DEBUGBIG",message);
}

void addPattern(uint8_t pseq, std::string pname, tPatternFunc pfunc)
{
    sPattern thispat;
    thispat.patternSeq = pseq;
    thispat.pattern = pname;
    thispat.patternFunc = pfunc;
    patternList.push_back(thispat);
}

void addColor(uint8_t pseq, uint8_t cseq, uint32_t pcolor)
{
    sColor thiscol;
    thiscol.patternSeq = pseq;
    thiscol.colorSeq = cseq;
    thiscol.pccolor = pcolor;
    colorList.push_back(thiscol);
}

void addDuration(uint8_t pseq, uint16_t dsec)
{
    sDuration thisdur;
    thisdur.patternSeq = pseq;
    thisdur.durationSec = dsec;
    durationList.push_back(thisdur);
}

void addSpeed(uint8_t pseq, uint16_t smsec)
{
    sSpeed thisspeed;
    thisspeed.patternSeq = pseq;
    thisspeed.speedMilliSec = smsec;
    speedList.push_back(thisspeed);
}

void addExtra(uint8_t pseq, std::string edata)
{
    sExtra thisextra;
    thisextra.patternSeq = pseq;
    thisextra.extraData = edata;
    extraList.push_back(thisextra);
}


void setup() {
  
  uint32_t seed = millis();
  srand(seed);
  //srand(time(NULL));
  delay(500); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, 0, NUM_LEDS_STRIP1).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN2,COLOR_ORDER>(leds, NUM_LEDS_STRIP1,NUM_LEDS_STRIP2).setCorrection(TypicalLEDStrip);
  
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  
  Particle.function("passdata",passData);
  Particle.function("nextpat",switchPattern); // manually switch to next pattern
  Particle.variable("debugvar", debugvar);

  cylondir = "O"; // O=Out, I=In;
  
  addPattern(0,"CY",cylon);
  addColor(0,0,Red); addColor(0,1,Green); addColor(0,2,White);
  addDuration(0,6);
  addPattern(1,"FW",fireworks);
  addColor(1,0,Red); addColor(1,1,White); addColor(1,2,Blue);
  addExtra(1,"3,10");
  addDuration(1,5);
  addPattern(2,"RN",rain);
  addColor(2,0,Blue);
  addDuration(2,6);
  addPattern(3,"LI",lightning);
  addDuration(3,10);
  addPattern(4,"SB",starburst);
  addColor(4,0,Red); addColor(4,1,White); addColor(4,2,Green);
  addColor(4,4,Blue); addColor(4,4,Purple);
  addDuration(4,6);
  addExtra(4,"5");
  addPattern(5,"FB",fireburst);
  addColor(5,0,Red);addColor(5,1,White);addColor(5,1,Blue);
  addDuration(5,6);
  addPattern(6,"RB",rainbow); addDuration(6,5);
//  addPattern(6,"FC",fallingconfetti); addDuration(6,5);
  addPattern(7,"CF",confetti); addDuration(7,5);
  addPattern(8,"RS",rainbowspikes); addDuration(8,6);
  addPattern(9,"FG",firegrav);
  addColor(9,0,Red); addColor(9,1,Purple); addColor(9,2,White); addColor(9,3,Blue);
  addDuration(9,7);
  addPattern(10,"JU",juggle); addDuration(10,7);
  addPattern(11,"SI",sinewave); addDuration(11,7);
  addPattern(12,"HB",heartbeat); addDuration(12,7);
  addPattern(13,"RF",rainbowfull); addDuration(13,7);
  totalPatternCount = 14;
  definedPatterns = 14;
  patternIdx = 0;
  loadPatternData();
  debug("Pattern Size %d",patternList.size());
  
}




void copyspokes()
{
    FastLED.clear();
    if (curpatternlen == 60) {
        for (uint8_t l=0;l<60;l++) { leds[l]=spoke[l]; }
        for (uint8_t l=0;l<60;l++) { leds[(SPOKELEN)+l]=spoke[l]; }
        for (uint8_t l=0;l<60;l++) { leds[(2*SPOKELEN)+l]=spoke[60-l-1]; }

        }
    else
    {
        for (uint8_t l=0;l<60;l++) { leds[l]=lamp[l]; }
        for (uint8_t l=0;l<60;l++) { leds[(SPOKELEN)+l]=lamp[l+60]; }
        for (uint8_t l=0;l<60;l++) { leds[(2*SPOKELEN)+l]=lamp[180-l-1]; }
//        for (uint8_t l=0;l<NUM_LEDS;l++) { leds[l] = lamp[l]; }    
    }
        
}

void solidspoke(CRGB scol)
{
    for (uint8_t i=0;i<SPOKELEN;i++) {spoke[i]=scol;}
}

void solidlamp(CRGB lcol)
{
    for (uint8_t i=0;i<NUM_LEDS;i++) {lamp[i]=lcol;}
}


int passData(String Command)
{
// Commands : 3 Chars - Main Command   [ PAU = Pause main loop, (start all with this),CLR = Clear, PAT = Pattern, TPC = Total Pattern Count,
//          COL = ColorParameter , DUR = Duration between patterns, SPD = Speed of Pattern. MOD = Mode, RES = Resume main loop (end with this)]
//            3 Chars - Sequence of Pattern or Total Pattern Count for TPC, Mode for MOD (AUT = Auto, MAN = Manual)
//            Variable :
//                2 Chars for PAT = name of Pattern
//                3 Chars for Sequence of Color, followed by 6 Chars for RGB Color in Hex
//                3 Chars for Seconds in Duration
//                3 Chars for Speed of Pattern in Microseconds
    std::string inputcmd;
    std::string mainCommand;
    std::string patternSeq;
    
    inputcmd = Command.toUpperCase().c_str();
    String debugmsg = Command;
    debugmsg = "pd cmd=" + debugmsg + ";";
    debugvar = debugvar + debugmsg;
//    debug(debugmsg,0);

    if (inputcmd.length() < 3) { return -1;}
    mainCommand = inputcmd.substr(0,3);
    if (mainCommand == "PAU")
    {
        processLoopEvents = false;
        return 0;
    }
    if (mainCommand == "CLR") 
    {
        patternList.clear();
        colorList.clear();
        durationList.clear();
        speedList.clear();
        definedPatterns=0;
        return 0;
    }
    if (mainCommand == "RES")
    {
        processLoopEvents = true;
        patternIdx=0;
        loadPatternData();
        return 0;
    }
    if (inputcmd.length() < 6) { return -1;}
    patternSeq = inputcmd.substr(3,3);
    if (mainCommand == "MOD")
    {
        std::string thisMode;
        thisMode = patternSeq;
        if (thisMode == "AUT") {switchMode = "A";}
        else if (thisMode == "MAN") {switchMode = "M";}
        return 0;
    }
    if (mainCommand == "TPC")
    {
        uint8_t thisPatternCount;
        thisPatternCount = atoi(patternSeq.c_str());
        totalPatternCount = thisPatternCount;
        return 0;
    }
    if (mainCommand == "PAT")
    {
        if (inputcmd.length() < 8) {return -1;}
        sPattern thisPatternStruct;
        std::string thisPattern;
        thisPattern = inputcmd.substr(6,2);
        thisPatternStruct.patternSeq = atoi(patternSeq.c_str());
        thisPatternStruct.pattern = thisPattern;
        if (thisPattern == "CY")
        { thisPatternStruct.patternFunc = cylon; }
        else if (thisPattern == "FW")
        { thisPatternStruct.patternFunc = fireworks; }
        else if (thisPattern == "FB")
        { thisPatternStruct.patternFunc = fireburst; }
        else if (thisPattern == "FG")
        { thisPatternStruct.patternFunc = firegrav;}
        else if (thisPattern == "SC")
        { thisPatternStruct.patternFunc = singlecolor; }
        else if (thisPattern == "TC")
        { thisPatternStruct.patternFunc = tricolor; }
        else if (thisPattern == "SB")
        { thisPatternStruct.patternFunc = starburst; }
        else if (thisPattern == "MB") 
        { thisPatternStruct.patternFunc = smoothburst; }
        else if (thisPattern == "BB")
        { thisPatternStruct.patternFunc = blendburst; }
        else if (thisPattern == "GR")
        { thisPatternStruct.patternFunc = grow; }
        else if (thisPattern == "RN")
        { thisPatternStruct.patternFunc = rain; }
        else if (thisPattern == "ST")
        { thisPatternStruct.patternFunc = storm; }
        else if (thisPattern == "NS") 
        { thisPatternStruct.patternFunc = noise; }
        else if (thisPattern == "CL") 
        { thisPatternStruct.patternFunc = cyclecolor; }
        else if (thisPattern == "LI")
        { thisPatternStruct.patternFunc = lightning; }
        else if (thisPattern == "BA")
        { thisPatternStruct.patternFunc = barber; }
        else if (thisPattern == "RO") 
        { thisPatternStruct.patternFunc = rotate; }
        else if (thisPattern == "CS") 
        { thisPatternStruct.patternFunc = colorsplotch; }
        else if (thisPattern == "RB")
        { thisPatternStruct.patternFunc = rainbow; }
        else if (thisPattern == "RG")
        { thisPatternStruct.patternFunc = rainbowWithGlitter; }
        else if (thisPattern == "RF")
        { thisPatternStruct.patternFunc = rainbowfull; }
        else if (thisPattern == "CF")
        { thisPatternStruct.patternFunc = confetti; }
        else if (thisPattern == "FC")
        { thisPatternStruct.patternFunc = fallingconfetti; }
        else if (thisPattern == "MC")
        { thisPatternStruct.patternFunc = multiconfetti; }
        else if (thisPattern == "CC")
        { thisPatternStruct.patternFunc = cycleconfetti; }
        else if (thisPattern == "RS")
        { thisPatternStruct.patternFunc = rainbowspikes; }
        else if (thisPattern == "JU")
        { thisPatternStruct.patternFunc = juggle; }
        else if (thisPattern == "SI")
        { thisPatternStruct.patternFunc = sinewave; }
        else if (thisPattern == "HB")
        { thisPatternStruct.patternFunc = heartbeat; }
        patternList.push_back(thisPatternStruct);
        std::sort(patternList.begin(),patternList.end());
        definedPatterns++;
        return 0;
    }
    if (mainCommand == "COL")
    {
        if (inputcmd.length() < 15) {return -1;}
        std::string thisColorSeq;
        std::string thisColor;
        sColor thisColorStruct;
        thisColorSeq = inputcmd.substr(6,3);
        thisColor = inputcmd.substr(9,6);
        thisColorStruct.patternSeq = atoi(patternSeq.c_str());
        thisColorStruct.colorSeq = atoi(thisColorSeq.c_str());
        thisColorStruct.pccolor = strtol(thisColor.c_str(),NULL,16);
        colorList.push_back(thisColorStruct);
        std::sort(colorList.begin(),colorList.end(),colorcomp);
        return 0;
    }
    if (mainCommand == "DUR")
    {
        if (inputcmd.length() < 9) {return -1;}
        std::string thisDuration;
        thisDuration = inputcmd.substr(6,3);
        uint8_t thisDurationSec;
        thisDurationSec = atoi(thisDuration.c_str());
        sDuration thisDurationStruct;
        thisDurationStruct.patternSeq = atoi(patternSeq.c_str());
        thisDurationStruct.durationSec = thisDurationSec;
        durationList.push_back(thisDurationStruct);
        std::sort(durationList.begin(),durationList.end());
        return 0;
    }
    if (mainCommand == "SPD")
    {
        if (inputcmd.length() < 9) {return -1;}
        std::string thisSpeed;
        thisSpeed = inputcmd.substr(6,3);
        uint16_t thisSpeedMilliSec;
        thisSpeedMilliSec = atoi(thisSpeed.c_str());
        sSpeed thisSpeedStruct;
        thisSpeedStruct.patternSeq = atoi(patternSeq.c_str());
        thisSpeedStruct.speedMilliSec = thisSpeedMilliSec;
        speedList.push_back(thisSpeedStruct);
        std::sort(speedList.begin(),speedList.end());
        return 0;
    }
    if (mainCommand == "XTR")
    {
        if (inputcmd.length()< 7) {return -1;}
        std::string thisExtras;
        thisExtras = inputcmd.substr(6,std::string::npos);
        sExtra thisExtraStruct;
        thisExtraStruct.patternSeq = atoi(patternSeq.c_str());
        thisExtraStruct.extraData = thisExtras;
        extraList.push_back(thisExtraStruct);
        std::sort(extraList.begin(),extraList.end());
        return 0;
    }
  return -1;    
}


void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  if (processLoopEvents) {
  patternList[patternIdx].patternFunc();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  if (newMinorEventMilli) {
      if (rainbowDirection == "B") { gHue++; } else {gHue--;}
      if(curPattern == "ST") {
         stormraintimeidx++;
         stormraintimeidx = stormraintimeidx % 90;
         stormlightningtimeidx++;
     }
  }

  if (newMajorEventMilli) {
    if (curPattern == "BB") 
    {
        uint8_t stripewidth=SPOKELEN/blendburstnumbands;
        //int stripewidth = (SPOKELEN/starburstnumbands) ;
        if(blendburstblendidx<3) {
            blendburstblendidx++;
        } else {
            blendburstblendidx=0;
            if(blendburstpixelidx<stripewidth-1) {
                blendburstpixelidx++;
            } else {
                blendburstpixelidx = 0;
                if(blendburstcoloridx>0) {
                    blendburstcoloridx--;
                } else {
                    blendburstcoloridx = blendburstarraysize-1;
                }
            }
        }
            //if(blendburstpixelidx>stripewidth-0.2) {
            //    blendburstpixelidx=0;
            //    if(blendburstcoloridx==0) {blendburstcoloridx = blendburstarraysize-1;}
            //    else {blendburstcoloridx--;}
            //}
            //else {blendburstpixelidx +=0.1; }
     }
     //else if(curPattern == "ST") {
     //    stormraintimeidx++;
     //    stormraintimeidx = stormraintimeidx % 90;
     //    stormlightningtimeidx++;
    // }
     else if(curPattern == "CL") {
         if(cyclecolorblendidx<63) {
             cyclecolorblendidx++;
         }
             else {
                 cyclecolorblendidx = 0;
                 if(cyclecolorcoloridx<cyclecolorarraysize-1) {
                     cyclecolorcoloridx++;
                 } else {
                     cyclecolorcoloridx = 0;
                 }
             }
     }
    else if(curPattern == "MC") {
          if(multiconfetticoloridx<multiconfettiarraysize-1) {
              multiconfetticoloridx++;
          }
          else {
              multiconfetticoloridx = 0;
          }
     }
     else if(curPattern == "FC") {
         fallingconfettiidx--;
         if (fallingconfettiidx <= 0) {
             fallingconfettiidx = SPOKELEN - fallingconfettirange;
             }
     }
     else if(curPattern == "FB") {
         if (fireBurstMode==upMode)
         {
             if (cybigidx < (SPOKELEN + 10)) {cybigidx++;} else 
             {
                 fireBurstMode=downMode;
                 fallingconfettiidx= SPOKELEN - fallingconfettirange;
                 cybigidx = 0;
                 solidlamp(CRGB::Black);
                 milliSecMajorEventDelay = 25;
                 newMajorEventMilli.setPeriod(milliSecMajorEventDelay);
             }
         }
         else
         {
             fallingconfettiidx--;
             if (fallingconfettiidx <= 0) {
                 fallingconfettiidx = SPOKELEN - fallingconfettirange;
                 fireBurstMode=upMode;
                 cybigidx = 0;
                 solidlamp(CRGB::Black);
                 if (cyloncoloridx < (cyloncolorarraysize - 1)) {cyloncoloridx++;} else {cyloncoloridx = 0;}
                 milliSecMajorEventDelay = 10;
                 newMajorEventMilli.setPeriod(milliSecMajorEventDelay);
                 }
         }
     }
    else if(curPattern == "CC") {
        if(cycleconfetticycleidx==cycleconfetticyclelength-1) {
            cycleconfetticycleidx = 0;
            if(cycleconfetticoloridx<cycleconfettiarraysize-1) {
                cycleconfetticoloridx++;
            }
            else {
                cycleconfetticoloridx=0;
            }
        }
        else {
            cycleconfetticycleidx++;
        }
        //debugbigmsg = debugbigmsg + String(cycleconfetticycleidx)+"-"+String(cycleconfetticoloridx)+";";
    }
    else if (curPattern == "FG") {
        if(firegravpixelidx<firegravstopidx&&firegravup==true) {
            if(firegravpixelidx ==0) {
                firegravstopidx = random8(32,52);
            }
            firegravpixelidx++;
        } else if(firegravup==true) {
            //debugbig("setting to down");
            firegravpixelidx=0;
            firegravup=false;
            //solidlamp(CRGB::Black);
            solidspoke(CRGB::Black);
            //firegravstopidx = rand()%30+30;
        }
    }
    else if (curPattern == "SB")
    {
        int stripewidth = (SPOKELEN/starburstnumbands) ;
        if ((starburstcoloridx > 0) && (starburstpixelidx == (stripewidth - 1))) {starburstcoloridx--;} else
        { if (starburstpixelidx == (stripewidth - 1)) {starburstcoloridx = (starburstarraysize - 1);} }
       if (starburstpixelidx < (stripewidth- 1)) {starburstpixelidx++;} else {starburstpixelidx=0;}
        
    }
    else if (curPattern == "BA")
    {
       // if (barberblendidx<63) {
        //    barberblendidx++;
        //} else {
         //   barberblendidx = 0;
            /*if(barbercoloridx>0) {
                barbercoloridx--;
            } else {
                barbercoloridx=2;
            }*/
        //}
        if(barberblendidx<3) {
            barberblendidx++;
        } else {
            barberblendidx=0;
            if(barberpixelidx<barberstripewidth-1) {
                barberpixelidx++;
            } else {
                barberpixelidx = 0;
                if(barbercoloridx>0) {
                    barbercoloridx--;
                } else {
                    barbercoloridx = 2;
                }
            }
        }
    }
    else if (curPattern == "RO")
    {
        if(rotateblendidx<31) {
            rotateblendidx++;
        } else {
            rotateblendidx =0;
            if(rotatecoloridx<2) {
                rotatecoloridx ++;
            } else {
                rotatecoloridx=0;
            }
        }
    }
    else if (curPattern == "MB") 
    {
        int stripewidth = (SPOKELEN/smoothburstnumbands) ;
        if ((smoothburstcoloridx > 0) && (smoothburstpixelidx == (stripewidth - 1))) {smoothburstcoloridx--;} else
        { if (smoothburstpixelidx == (stripewidth - 1)) {smoothburstcoloridx = (smoothburstarraysize - 1);} }
       if (smoothburstpixelidx < (stripewidth- 1)) {smoothburstpixelidx++;} else {smoothburstpixelidx=0;}
    }
    else if (curPattern == "FW")
    {
        int stripewidth = (SPOKELEN/fireworkbands) - 1;
        if ((fireworkcoloridx > 0) && (fireworkpixelidx == stripewidth)) {fireworkcoloridx--;} else
        { if (fireworkpixelidx == stripewidth) {fireworkcoloridx = (fireworkcolorarraysize - 1);} }
       if (fireworkpixelidx < stripewidth) {fireworkpixelidx++;} else {fireworkpixelidx=0;}
        debug("TPC: %d",CLOCKS_PER_SEC);
    }
    else if (curPattern == "GR") 
    {
        growtimeidx+=growdirection;
        if(growtimeidx==63&&growdirection==1) {
            growdirection=-1;
        }
        if(growtimeidx==0&&growdirection==-1){
            growdirection=1;
            if(growcoloridx<growcolorarraysize-1) {
                growcoloridx++;
            }
            else {
                growcoloridx=0;
            }
        }
            
    }
    else if (curPattern == "CY")
    {
        if(cylondir == "O") {
            if (cybigidx < (SPOKELEN + 10)) {cybigidx++;} else
            {
                cylondir = "I";
            }
        }
        if(cylondir == "I") {
            if (cybigidx > 0) {cybigidx--;} else
            {
                cylondir = "O";
                if (cyloncoloridx < (cyloncolorarraysize - 1)) {cyloncoloridx++;} else {cyloncoloridx = 0;}
        }
        }    
    }
  }    
  if ( switchMode == "A") { if (newPatternTime) { nextPattern(); } }
  }
}


int switchPattern(String Command)
{
    nextPattern();
    return 0;
}

void findColorSubList(uint8_t pseq)
{
    colorSubList.clear();
    for(std::vector<sColor>::size_type i = 0; i != colorList.size(); i++)
        {
            if (colorList[i].patternSeq == pseq) {
                debugvar=debugvar+"pccolor["+String(i)+"]="+String(colorList[i].pccolor)+";";
                colorSubList.push_back(CRGB(colorList[i].pccolor));
            }
            
        }
}

void findDuration(uint8_t pseq)
{
    bool durationFound = false;
    for(std::vector<sColor>::size_type i = 0; i != durationList.size(); i++)
        {
            if (durationList[i].patternSeq == pseq) {
                durationFound = true;
                secondsBetweenPatterns = durationList[i].durationSec;
            }
        }    
    if (!durationFound) {secondsBetweenPatterns = defaultSecondsBetweenPatterns;}
    newPatternTime.setPeriod(secondsBetweenPatterns);
}

bool findSpeed(uint8_t pseq)
{
    bool speedFound = false;
   for(std::vector<sColor>::size_type i = 0; i != speedList.size(); i++)
        {
            if (speedList[i].patternSeq == pseq) {
                speedFound = true;
                milliSecMajorEventDelay = speedList[i].speedMilliSec;
            }
        }    
   return speedFound; 
}

std::string getExtra(uint8_t pseq)
{
    std::string extraReturn = "";
    for(std::vector<sColor>::size_type i=0;i != extraList.size(); i++)
    {
        if(extraList[i].patternSeq == pseq) {
            extraReturn = extraList[i].extraData;
        }
    }
    return extraReturn;
}


void loadPatternData()
{
  curPattern = "";
  curPattern = patternList[patternIdx].pattern;
  debugvar=debugvar+"newcp=";
  debugvar=debugvar+curPattern.c_str();
  debugvar=debugvar+";";
  findDuration(patternList[patternIdx].patternSeq);
  bool speedFound = findSpeed(patternList[patternIdx].patternSeq);
  if (curPattern == "SC") {
        findColorSubList(patternList[patternIdx].patternSeq);
        debugvar=debugvar+"cslsize="+String(colorSubList.size())+";";
        if (colorSubList.size() > 0 ) {
            colorsinglecolor = colorSubList[0];
            debugvar=debugvar+"scred="+String(colorsinglecolor.red)+";";
            debugvar=debugvar+"scgreen="+String(colorsinglecolor.green)+";";
            debugvar=debugvar+"scblue="+String(colorsinglecolor.blue)+";";
        }
  }
  else if (curPattern == "TC") {
        findColorSubList(patternList[patternIdx].patternSeq);
        if (colorSubList.size() > 1) {
            color3colors[0] = colorSubList[0];
            color3colors[1] = colorSubList[1];
            color3colors[2] = colorSubList[2];
        }    
  }
  else if (curPattern == "CY") {
      cylondir = "O";
      cybigidx = 0;
      cyloncoloridx = 0;
      findColorSubList(patternList[patternIdx].patternSeq);
      cylonColorList = colorSubList;
      cyloncolorarraysize = cylonColorList.size();
      if (!speedFound) {milliSecMajorEventDelay = 10;}
      newMajorEventMilli.setPeriod(milliSecMajorEventDelay);
  }
  else if (curPattern == "FW") {
      findColorSubList(patternList[patternIdx].patternSeq);
      fireworkColorList = colorSubList;
      fireworkcolorarraysize = fireworkColorList.size();
      fireworkcoloridx = fireworkcolorarraysize;
      fireworkExtraData = getExtra(patternList[patternIdx].patternSeq);
      fireworkbands = 2;
      fireworklen = 10;
      if (!fireworkExtraData.empty()) {
          uint8_t commapos;
          commapos = fireworkExtraData.find(",");
          if (commapos != std::string::npos) {
              std::string bandstring;
              std::string lenstring;
              bandstring = fireworkExtraData.substr(0,commapos);
              if (fireworkExtraData.length() > commapos+1) {
                  lenstring = fireworkExtraData.substr(commapos+1,std::string::npos);
                  fireworkbands = atoi(bandstring.c_str());
                  fireworklen = atoi(lenstring.c_str());
              }
          }
      }
      if (!speedFound) {milliSecMajorEventDelay = 10;}
      newMajorEventMilli.setPeriod(milliSecMajorEventDelay);
  }
  else if (curPattern == "SB" ) {
      starburstcoloridx = 0;
      starburstpixelidx = 0;
      findColorSubList(patternList[patternIdx].patternSeq);
      starburstColorList = colorSubList;
      starburstarraysize = starburstColorList.size();
      starburstnumbands = atoi(extraParse(getExtra(patternList[patternIdx].patternSeq),",")[0].c_str());
      if (!speedFound) {milliSecMajorEventDelay = 100;}
      newMajorEventMilli.setPeriod(milliSecMajorEventDelay);
      
  }
  else if (curPattern == "BA") {
      barbercoloridx = 0;
      barberblendidx = 0;
      barberpixelidx = 0;
      findColorSubList(patternList[patternIdx].patternSeq);
      barberColorList = colorSubList;
      if(!speedFound) {milliSecMajorEventDelay = 1;}
      newMajorEventMilli.setPeriod(milliSecMajorEventDelay);
  }
  else if (curPattern == "MB") {
      smoothburstcoloridx = 0;
      smoothburstpixelidx = 0;
      findColorSubList(patternList[patternIdx].patternSeq);
      smoothburstColorList = colorSubList;
      smoothburstnumbands = atoi(extraParse(getExtra(patternList[patternIdx].patternSeq),",")[0].c_str());
      smoothburstarraysize = smoothburstColorList.size();
      if (!speedFound) {milliSecMajorEventDelay = 100;}
      newMajorEventMilli.setPeriod(milliSecMajorEventDelay);
  }
  else if (curPattern == "BB") {
      blendburstcoloridx = 0;
      blendburstpixelidx = 0;
      blendburstblendidx = 0;
      findColorSubList(patternList[patternIdx].patternSeq);
      blendburstColorList = colorSubList;
      blendburstarraysize = blendburstColorList.size();
      blendburstnumbands = atoi(extraParse(getExtra(patternList[patternIdx].patternSeq),",")[0].c_str());
      if(!speedFound) {milliSecMajorEventDelay = 1;}
      newMajorEventMilli.setPeriod(milliSecMajorEventDelay);
  }
  else if (curPattern == "RO") {
      rotatecoloridx = 0;
      rotateblendidx = 0;
      findColorSubList(patternList[patternIdx].patternSeq);
      rotateColorList = colorSubList;
      if(!speedFound) {milliSecMajorEventDelay = 5;}
      newMajorEventMilli.setPeriod(milliSecMajorEventDelay);
  }
  else if (curPattern == "CL") {
      cyclecolorcoloridx = 0;
      cyclecolorblendidx = 0;
      findColorSubList(patternList[patternIdx].patternSeq);
      cyclecolorColorList = colorSubList;
      cyclecolorarraysize = cyclecolorColorList.size();
      if(!speedFound) {milliSecMajorEventDelay = 5;}
      newMajorEventMilli.setPeriod(milliSecMajorEventDelay);
  }
  else if (curPattern == "GR") {
      growcoloridx = 0;
      growtimeidx = 0;
      growdirection =1;
      findColorSubList(patternList[patternIdx].patternSeq);
      growColorList = colorSubList;
      growcolorarraysize = growColorList.size();
      if (!speedFound) {milliSecMajorEventDelay = 25;}
      newMajorEventMilli.setPeriod(milliSecMajorEventDelay);
  }
  else if (curPattern == "CS") {
      colorSplotches = {};
      if (!speedFound) {milliSecMajorEventDelay = 10;}
      newMajorEventMilli.setPeriod(milliSecMajorEventDelay);
  }
  else if (curPattern == "MC") {
      multiconfetticoloridx = 0;
      findColorSubList(patternList[patternIdx].patternSeq);
      multiconfettiColorList = colorSubList;
      multiconfettiarraysize = multiconfettiColorList.size();
      if(!speedFound) {milliSecMajorEventDelay = 5;}
      newMajorEventMilli.setPeriod(milliSecMajorEventDelay);
      
  }
  else if (curPattern == "FC") {
      fallingconfettiidx = SPOKELEN-fallingconfettirange;
      if(!speedFound) {milliSecMajorEventDelay = 30;}
      newMajorEventMilli.setPeriod(milliSecMajorEventDelay);
  }
  else if (curPattern == "FB") {
      fireBurstMode = upMode;
      cybigidx = 0;
      cyloncoloridx = 0;
      findColorSubList(patternList[patternIdx].patternSeq);
      cylonColorList = colorSubList;
      cyloncolorarraysize = cylonColorList.size();
      fallingconfettiidx = SPOKELEN-fallingconfettiidx;
      if (!speedFound) {milliSecMajorEventDelay = 10;}
      newMajorEventMilli.setPeriod(milliSecMajorEventDelay);
  }
  else if (curPattern == "FG") {
      solidspoke(CRGB::Black);
      firegravup = true;
      firegravcoloridx = 0;
      firegravpixelidx = 0;
      firegravparticles = {};
      findColorSubList(patternList[patternIdx].patternSeq);
      firegravColorList = colorSubList;
      firegravcolorarraysize = firegravColorList.size();
      if (!speedFound) {milliSecMajorEventDelay = 10;}
      newMajorEventMilli.setPeriod(milliSecMajorEventDelay);
  }
  else if (curPattern == "RN") {
      rainDrops1 = {};
      rainDrops2 = {};
      rainDrops3 = {};
      findColorSubList(patternList[patternIdx].patternSeq);
      raindropcolor = colorSubList[0];
      //std::string teststring = extraParse("hi,hello",",")[0];
      //debugbig(teststring.c_str());
      //debugbig("aflskdfjsd");
    if(!speedFound) {milliSecMajorEventDelay = 90;}
      newMajorEventMilli.setPeriod(milliSecMajorEventDelay);
  }
  else if (curPattern == "ST") {
      stormDrops1 = {};
      stormDrops2 = {};
      stormDrops3 = {};
      stormraintimeidx = 0;
      stormlightningtimeidx = 0;
      if(!speedFound) {milliSecMinorEventDelay = 1;}
      newMinorEventMilli.setPeriod(milliSecMajorEventDelay);
  }
  else if (curPattern == "NS") {
    if(!speedFound) {milliSecMajorEventDelay = 60;}
    newMajorEventMilli.setPeriod(milliSecMajorEventDelay);
  }
  else if (curPattern == "CC") {
      cycleconfetticoloridx = 0;
      cycleconfetticycleidx = 0;
      findColorSubList(patternList[patternIdx].patternSeq);
      cycleconfettiColorList = colorSubList;
      cycleconfettiarraysize = cycleconfettiColorList.size();
      if(!speedFound) {milliSecMajorEventDelay = 5;}
      newMajorEventMilli.setPeriod(milliSecMajorEventDelay);
  }
  newPatternTime.reset();
    
}

void nextPattern()

{
  //debugbig(debugbigmsg);    
  debugbigmsg="";    
  // add one to the current pattern number, and wrap around at the end
  patternIdx++;
//  debug("in NextPattern, proposed patternIdx %d",patternIdx);
  if (patternIdx >= patternList.size()) { patternIdx = 0; }
//  debug("in NextPattern, revised proposed patternIdx %d", patternIdx);
  //String debugmsg = patternList[patternIdx].pattern.c_str();
//  debugmsg = "In Next pattern, proposed pattern" + debugmsg;
//  debug(debugmsg,0);
  loadPatternData();
  FastLED.clear();
  FastLED.show();
  
}

std::vector<std::string> extraParse(std::string extratext,std::string delimiter) {
    //debugbig("parsing");
    std::vector<std::string> retarray;
    size_t current;
    size_t next = -1;
    //debugbig("set vars");
    do {
        current = next + 1;
        next = extratext.find_first_of(delimiter,current);
        if(next!=std::string::npos) {retarray.push_back(extratext.substr(current,next-current));} else {retarray.push_back(extratext.substr(current,extratext.length()-current)); }
    } while (next != std::string::npos);
    //debugbig(retarray[0].c_str());
    //debugbig( retarray[1].c_str());
    return retarray;
    //debugbig("finished loop");
}


void singlecolor()
{
    curpatternlen=180;
    solidlamp(colorsinglecolor);
    copyspokes();
  //fill_solid( lamp, NUM_LEDS, colorsinglecolor);
  //curpatternlen =180;
  //copyspokes();
}


void tricolor()
{
    curpatternlen=180;
    for (int l=0; l<SPOKELEN; l++) {
        lamp[l]=color3colors[0];
    }
    for (int l=0; l<SPOKELEN; l++) {
        lamp[(SPOKELEN)+l]=color3colors[1];
    }
    for (int l=0; l<SPOKELEN; l++) {
        lamp[(2*SPOKELEN)+l]=color3colors[2];
    }
    copyspokes();
}

void cylon()
{
    curpatternlen=60;
    fill_solid(spoke,SPOKELEN,CRGB::Black);
    int lowled = min(max(cybigidx-10,0),SPOKELEN);
    int highled = min(cybigidx,SPOKELEN-1);
    for (int i=lowled;i<=highled;i++) {spoke[i]=cylonColorList[cyloncoloridx];}
    copyspokes();
}

void rainbow() 
{
    curpatternlen=180;
  // FastLED's built-in rainbow generator
  fill_rainbow( lamp, NUM_LEDS, gHue, 2);
  copyspokes();
}

void rainbowspikes()
{
    curpatternlen=60;
    fill_rainbow(spoke, SPOKELEN, gHue, 4);
    copyspokes();
}

void rainbowfull()
{
    curpatternlen=180;
    fill_solid(lamp, NUM_LEDS, CHSV(gHue,255,255));
    copyspokes();
}


void starburst()
{
    curpatternlen=60;
    int stripewidth = SPOKELEN/starburstnumbands;
    for (int i=0;i<SPOKELEN;i++)
    {
        int thiscolorindex = ((int(floor(float((i - starburstpixelidx))/float(stripewidth)))+starburstcoloridx+1))%starburstarraysize;
        spoke[i]=starburstColorList[thiscolorindex];
    }
    copyspokes();

}

void smoothburst() 
{
    curpatternlen=60;
    int stripewidth = SPOKELEN/smoothburstnumbands;
    for (int i=0;i<SPOKELEN;i++)
    {
        int thiscolorindex = ((int(floor(float((i - smoothburstpixelidx))/float(stripewidth)))+smoothburstcoloridx+1))%smoothburstarraysize;
        float fractionpart = (float((i - smoothburstpixelidx))/float(stripewidth)) - floor(float((i - smoothburstpixelidx))/float(stripewidth));
        CRGB color1 = smoothburstColorList[(thiscolorindex+1)%smoothburstarraysize];
        CRGB color2 = smoothburstColorList[thiscolorindex];
        CRGB currentcolor = blend(color2,color1,int(fractionpart*256));
        spoke[i]=currentcolor;
    }
    copyspokes();
}

void fireworks()
{
    curpatternlen=60;
    int stripewidth = SPOKELEN/fireworkbands;
    for (int i=0;i<SPOKELEN;i++)
    {
        if((i-fireworkpixelidx+stripewidth)%stripewidth<fireworklen) {
            int thiscolorindex = ((int(floor(float((i - fireworkpixelidx))/float(stripewidth)))+fireworkcoloridx+1))%fireworkcolorarraysize;
            spoke[i]=fireworkColorList[thiscolorindex];
        }
        else
        {
            spoke[i]=CRGB::Black;    
        }
    }
    copyspokes();

}

void blendburst() {
    curpatternlen=60;
    int stripewidth = SPOKELEN/blendburstnumbands;
    for (int i=0;i<SPOKELEN;i++)
    {
        int thiscolorindex = ((int(floor(float((i - blendburstpixelidx))/float(stripewidth)))+blendburstcoloridx+1))%blendburstarraysize;
        if( (i-blendburstpixelidx)%stripewidth==0) {
            CRGB thiscolor = blend(blendburstColorList[thiscolorindex],blendburstColorList[(thiscolorindex-1+blendburstarraysize)%blendburstarraysize],blendburstblendidx*64);
            spoke[i] = thiscolor;
        } else {
            spoke[i]=blendburstColorList[thiscolorindex];
        }
       
    }
    copyspokes();

     /*curpatternlen=60;
      float intermediate = blendburstpixelidx - floor(blendburstpixelidx);
      uint8_t intpixelidx = (int) floor(blendburstpixelidx);
      int width = curpatternlen/blendburstnumbands;
      for (int i=0;i<intpixelidx-1;i++)  { spoke[i]=blendburstColorList[blendburstcoloridx]; }
      spoke[intpixelidx-1] = myblend(intermediate, blendburstColorList[blendburstcoloridx],blendburstColorList[(blendburstcoloridx+1)%blendburstarraysize]);
      for (int i=1;i<blendburstnumbands;i++) {
        for (int j=0;j<=width-1;j++) {
          spoke[(i-1)*width+intpixelidx+j]=blendburstColorList[(blendburstcoloridx+i)%blendburstarraysize];
        }
        spoke[i*width+intpixelidx-1]=myblend(intermediate, blendburstColorList[blendburstcoloridx+i],blendburstColorList[(blendburstcoloridx+i+1)%blendburstarraysize]);
      }
      for (int i=0;i<width-intpixelidx;i++) {
        spoke[(blendburstnumbands-1)*width+intpixelidx+i]=blendburstColorList[(blendburstcoloridx+blendburstnumbands)%blendburstarraysize];
      }
      copyspokes();*/
      
 
  
}





CRGB myblend(float blendvalue, CRGB color1, CRGB color2) {
CRGB retcolor;

retcolor.red = blendvalue*color1.red + (1-blendvalue)*color2.red;
retcolor.green = blendvalue*color1.green + (1-blendvalue)*color2.green;
retcolor.blue = blendvalue*color1.blue + (1-blendvalue)*color2.blue;
//debugbigmsg = debugbigmsg + String(color1.red) + "-" + String(color1.green)+"-"+String(color1.blue)+","+String(retcolor.red)+"-"+String(retcolor.green)+"-"+String(retcolor.blue)+","+String(color2.red)+"-"+String(color2.green)+"-"+String(color2.blue)+";";
return retcolor;
}



void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
 curpatternlen=180;
  rainbow();
  addGlitter();
  copyspokes();
}

void addGlitter() 
{
  if( random8() < 80 ) {
    lamp[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  curpatternlen=180;
  fadeToBlackBy( lamp, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  lamp[pos] += CHSV( gHue + random8(64), 200, 255);
  copyspokes();
}

void fallingconfetti() {
  curpatternlen=180;
  debugbigmsg=debugbigmsg+"fci:"+fallingconfettiidx;
  fadeToBlackBy( lamp, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
//  lamp[pos] += CHSV ( 100, 200, 255);
  lamp[pos] += CHSV( gHue + random8(64), 200, 255);
  for(uint8_t s=0;s<3;s++)
  {
      for(uint8_t i=0;i<min(fallingconfettiidx,SPOKELEN);i++)
      {
          lamp[(s*SPOKELEN)+i]=CRGB::Black;
      }
      for(uint8_t i=min(fallingconfettiidx+fallingconfettirange,SPOKELEN);i<SPOKELEN;i++)
      {
          lamp[(s*SPOKELEN)+i]=CRGB::Black;
      }
  }
  copyspokes();
    
}
  

void fireburst()
{
    if(fireBurstMode==upMode) {
    curpatternlen=60;
    fill_solid(spoke,SPOKELEN,CRGB::Black);
    int lowled = min(max(cybigidx-10,0),SPOKELEN);
    int highled = min(cybigidx,SPOKELEN-1);
    for (int i=lowled;i<=highled;i++) {spoke[i]=cylonColorList[cyloncoloridx];}
    }
    else
    {
     curpatternlen=180;
     fadeToBlackBy( lamp, NUM_LEDS, 10);
     int randtest = rand() % 3;
     if (randtest == 0)
    {
         int pos = random16(fallingconfettirange)+fallingconfettiidx;
         CHSV newshade;
         newshade = rgb2hsv_approximate(cylonColorList[cyloncoloridx]);
         lamp[pos].setHSV(newshade.h+random8(80)-40,newshade.s,newshade.v);
         pos = SPOKELEN + random16(fallingconfettirange)+fallingconfettiidx;
         lamp[pos].setHSV(newshade.h+random8(80)-40,newshade.s,newshade.v);
         pos = 2*SPOKELEN + random16(fallingconfettirange)+fallingconfettiidx;
         lamp[pos].setHSV(newshade.h+random8(80)-40,newshade.s,newshade.v);
    }
//    for(uint8_t s=0;s<3;s++)
//     {
//          for(uint8_t i=0;i<min(fallingconfettiidx,SPOKELEN);i++)
//        {
//          lamp[(s*SPOKELEN)+i]=CRGB::Black;
//         }
//        for(uint8_t i=min(fallingconfettiidx+fallingconfettirange,SPOKELEN);i<SPOKELEN;i++)
//        {
//          lamp[(s*SPOKELEN)+i]=CRGB::Black;
//          }
//        }        
    }
    copyspokes();
}



void multiconfetti() {
    curpatternlen=180;
    fadeToBlackBy(lamp, NUM_LEDS, 10);
    int pos = random16(NUM_LEDS);
    lamp[pos] += multiconfettiColorList[multiconfetticoloridx];
}

void cycleconfetti() {
    curpatternlen=180;
    fadeToBlackBy(lamp,NUM_LEDS,10);
    int pos = random16(NUM_LEDS);
    lamp[pos] += cycleconfettiColorList[cycleconfetticoloridx];
    copyspokes();
}

void sinewave()
{
  // a colored dot sweeping back and forth, with fading trails
  curpatternlen=60;
  fadeToBlackBy( spoke, SPOKELEN, 20);
  int pos = beatsin16(13,0,SPOKELEN);
  spoke[pos] += CHSV( gHue, 255, 192);
  copyspokes();
}

void heartbeat()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  curpatternlen=60;
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < SPOKELEN; i++) { //9948
    spoke[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
  copyspokes();
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  curpatternlen=60;
  fadeToBlackBy( spoke, SPOKELEN, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    spoke[beatsin16(i+7,0,SPOKELEN)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  copyspokes();
}

void rain() {
  fadeToBlackBy(lamp, NUM_LEDS, 125);    
  curpatternlen = 180;
  //int curNum = rand() % 10;
  for(std::vector<std::vector<sRaindrop>>::size_type k=0;k<rainDropSpokes.size();k++) {
    //std::vector<sRaindrop> rainDrops = rainDropSpokes[k];
    for(std::vector<sRaindrop>::size_type i=0;i<rainDropSpokes[k].size();i++) {
        rainDropSpokes[k][i].velocity += gravity;
        rainDropSpokes[k][i].lastPos = rainDropSpokes[k][i].position;
        rainDropSpokes[k][i].position -= rainDropSpokes[k][i].velocity;
        if (rainDropSpokes[k][i].lastPos> 0) {
            int lowerbound = max(int(rainDropSpokes[k][i].position),0);
            int upperbound = rainDropSpokes[k][i].lastPos;
            if(upperbound == lowerbound) {upperbound++;}
            for(int j=lowerbound;j<upperbound;j++) {
                lamp[SPOKELEN*k+j] = raindropcolor;
                //spoke[j] = CRGB::Blue;
                
            }
        } else {
            rainDropSpokes[k].erase(rainDropSpokes[k].begin()+i);
        }
    }
      int curNum = rand() % 10;
      if(curNum==0) {
        sRaindrop newDrop;
        newDrop.position = SPOKELEN-1;
        newDrop.velocity = 0;
        newDrop.lastPos = SPOKELEN-1;
        rainDropSpokes[k].push_back(newDrop);
      }
    copyspokes();
  }
 
}

void lightning()
{
  uint8_t ledstart = random8(NUM_LEDS);           // Determine starting location of flash
  uint8_t ledlen = random8(NUM_LEDS-ledstart);    // Determine length of flash (not to go beyond NUM_LEDS-1)
  for (int flashCounter = 0; flashCounter < random8(3,lightningFlashes); flashCounter++) {
    if(flashCounter == 0) lightningDimmer = 5;     // the brightness of the leader is scaled down by a factor of 5
    else lightningDimmer = random8(1,3);           // return strokes are brighter than the leader
    fill_solid(leds+ledstart,ledlen,CHSV(255, 0, 255/lightningDimmer));
    FastLED.show();                       // Show a section of LED's
    delay(random8(4,10));                 // each flash only lasts 4-10 milliseconds
    fill_solid(leds+ledstart,ledlen,CHSV(255,0,0));   // Clear the section of LED's
    FastLED.show();     
    if (flashCounter == 0) delay (150);   // longer delay until next flash after the leader
    delay(50+random8(100));               // shorter delay between strokes  
  } // for()
  delay(random8(lightningFrequency)*100);          // delay between strikes    
}

void storm() {
    curpatternlen=180;
    //rain
    fadeToBlackBy(stormrainLEDS, NUM_LEDS, 125);    
    //curpatternlen = 180;
    //int curNum = rand() % 10;
    for(std::vector<std::vector<sRaindrop>>::size_type k=0;k<stormDropSpokes.size();k++) {
    //std::vector<sRaindrop> rainDrops = rainDropSpokes[k];
    for(std::vector<sRaindrop>::size_type i=0;i<stormDropSpokes[k].size();i++) {
        stormDropSpokes[k][i].velocity += gravity;
        stormDropSpokes[k][i].lastPos = stormDropSpokes[k][i].position;
        stormDropSpokes[k][i].position -= stormDropSpokes[k][i].velocity;
        if (stormDropSpokes[k][i].lastPos> 0) {
            int lowerbound = max(int(stormDropSpokes[k][i].position),0);
            int upperbound = stormDropSpokes[k][i].lastPos;
            if(upperbound == lowerbound) {upperbound++;}
            for(int j=lowerbound;j<upperbound;j++) {
                //lamp[SPOKELEN*k+j] = raindropcolor;
                stormrainLEDS[SPOKELEN*k+j] = CRGB::Blue;
                //lamp[SPOKELEN*k+j] = CRGB::Blue;
                //spoke[j] = CRGB::Blue;
                
            }
        } else {
            stormDropSpokes[k].erase(stormDropSpokes[k].begin()+i);
        }
    }
      int curNum = rand() % 10;
      if(curNum==0) {
        sRaindrop newDrop;
        newDrop.position = SPOKELEN-1;
        newDrop.velocity = 0;
        newDrop.lastPos = SPOKELEN-1;
        stormDropSpokes[k].push_back(newDrop);
      }
    //copyspokes();
  }
    //lightning
    if(stormlightningtimeidx==0) {
        currentstormstart = random8(NUM_LEDS);
        currentstormend = currentstormstart + random8(NUM_LEDS-currentstormstart);
        stormnumflashes = random8(3,stormFlashes);
    }
    
    if(stormlightningtimeidx == stormnextflashidx) {
        debugbig("flashing");
        stormflashidx++;
        if(stormflashidx ==0) {
            stormDimmer = 5;
        } else {
            stormDimmer = random8(1,3);
        }
       CHSV flashcolorh = CHSV(255,0,255/stormDimmer);
       CRGB flashcolor = flashcolorh;
       for(int i=currentstormstart;i<=currentstormend;i++) {
           stormlightningLEDS[i] = flashcolor; 
           //lamp[i] = flashcolor;
       }
       stormendflashidx = stormlightningtimeidx + random8(4,10);
    }
    if(stormlightningtimeidx==stormendflashidx) {
        debugbig("ending the flash");
        for(int i = currentstormstart;i<=currentstormend;i++) {
            stormlightningLEDS[i] = CRGB::Black;
        }
        if(stormflashidx==stormFlashes) {
            stormnextflashidx = random8(stormFrequency)*100;
            stormendflashidx = stormnextflashidx;
            stormlightningtimeidx = 0;
            stormflashidx = 0;
        } else {
            if(stormflashidx ==0) {
                stormnextflashidx = stormlightningtimeidx + 150;
            } else {
                stormnextflashidx = stormlightningtimeidx + 50 + random8(100);
            }
        }
    }
    for(int i=0;i<NUM_LEDS;i++) {
        lamp[i] = blend(stormrainLEDS[i],stormlightningLEDS[i],128);
    }
    //stormnextflashidx = stormflashidx + random8(4,10);
    copyspokes();
    
}

void firegrav() {
    

    if(firegravup == true) {
        curpatternlen = 60;
        fadeToBlackBy(spoke,SPOKELEN,50);
        spoke[firegravpixelidx] = firegravColorList[firegravcoloridx];
        //debugbig("going up");
    } else {
        curpatternlen = 180;
       // debugbig("going down");
        fadeToBlackBy(lamp,NUM_LEDS,200);
        if(firegravparticles.size()==0) {
           // debugbig("exploding");
            for(int i=0;i<22;i++) {
                sParticle newParticle;
                //newParticle.position = (rand()%15)+45;
                newParticle.position = firegravstopidx;
                newParticle.lastPos = newParticle.position;
                newParticle.velocity = float(rand()%6-3)/float(3);
                newParticle.spoke = rand()%3;
                //newParticle.duration =0;
                firegravparticles.push_back(newParticle);
            }
        }
        int numinbounds = 0;
        //debug("size: %d", firegravparticles.size());
        for(std::vector<sParticle>::size_type i=0;i<firegravparticles.size();i++) {
            //debugbig("showing");
            firegravparticles[i].velocity+=gravity;
            firegravparticles[i].lastPos=firegravparticles[i].position;
            firegravparticles[i].position-=firegravparticles[i].velocity;
            
            if(firegravparticles[i].lastPos>0) {numinbounds++;}
            if(firegravparticles[i].lastPos>0&&firegravparticles[i].position<SPOKELEN) {
                //numinbounds++;
                int lowerbound = max(int(firegravparticles[i].position),0);
                int upperbound = min(int(firegravparticles[i].lastPos),SPOKELEN-1);
                if(upperbound == lowerbound) {upperbound++;}
                for (int j=lowerbound;j<upperbound;j++) {
                    lamp[j+SPOKELEN*firegravparticles[i].spoke] = firegravColorList[firegravcoloridx];
                }
            }
        }
        if(numinbounds ==0) {
            firegravup=true;
            firegravpixelidx=0;
            firegravparticles={};
            if(firegravcoloridx<firegravcolorarraysize-1) {
                firegravcoloridx++;
            } else {
                firegravcoloridx=0;
            }
        }
    }
    copyspokes();
}

void grow() {
    curpatternlen=60;
    CHSV topHalf[SPOKELEN/2];
    solidspoke(CRGB::Black);
    CHSV currentcolor = rgb2hsv_approximate(growColorList[growcoloridx]);
    for(int i=0;i<SPOKELEN/2;i++) {
        topHalf[i].setHSV(currentcolor.hue,currentcolor.saturation,max(0,(i+growtimeidx-29)*4));
    }
    for(int i=0;i<SPOKELEN;i++) {
        if(i<30) {
            spoke[i]=topHalf[i];
        }
        else {
            spoke[i] = topHalf[59-i];
        }
    }
    copyspokes();
}

void colorsplotch() {
    curpatternlen = 180;
    //std::vector<CHSV> currentspoke;
    solidspoke(CRGB::Black);
    int randNum = rand() % 4;
    if(randNum == 0) {
        sSplotch newSplotch;
        newSplotch.position = rand() % 180;
        //debug("position: %d",newSplotch.position);
        //newSplotch.duration = 0;
        newSplotch.growth = 0;
        newSplotch.huevalue = rand() % 256;
        //newSplotch.centervalue = 255;
        //newSplotch.splotchvector = {255};
        colorSplotches.push_back(newSplotch);
        //debugbig("making a splotch");
    }
    for (std::vector<sSplotch>::size_type i = 0;i<colorSplotches.size();i++) {
        if(colorSplotches[i].growth<20) {
            colorSplotches[i].growth++;
            //colorSplotches[i].splotchvector.push_back(0);
            //debug("splotchvector size: %d",colorSplotches[i].splotchvector.size());
            //std::vector<int> oldsplotchvector = colorSplotches[i].splotchvector;
            //for(std::vector<int>::size_type j =0;j<colorSplotches[i].splotchvector.size()-2;j++) {
              //  int movepart = int(0.4*oldsplotchvector[j]);
                //colorSplotches[i].splotchvector[j]-=movepart;
                //colorSplotches[i].splotchvector[j+1]+=movepart;
                //debugbig("next moves");
            //}
            //if(colorSplotches[i].splotchvector.size()==2) {
            //    int movepart = int(0.4*oldsplotchvector[0]);
            //    //colorSplotches[i].splotchvector[0]-=movepart;
            //    colorSplotches[i].splotchvector[1]+=movepart;
                //debugbig("first move");
            //}
            //if(i=0) { debug("last: %d",colorSplotches[i].splotchvector[colorSplotches[i].splotchvector.size()-1]);}
            //colorSplotches[i].centervalue = colorSplotches[i].splotchvector[0];
            //debug("center value: %d",colorSplotches[i].centervalue);
            CHSV splotchcolorh = CHSV(colorSplotches[i].huevalue,255,255);
            CRGB splotchcolor = splotchcolorh;
            int splotchvalue = max(max(splotchcolor.red,splotchcolor.green),splotchcolor.blue);
            for (int j = 0;j<NUM_LEDS;j++) {
                int splotchdistance = abs(j - colorSplotches[i].position);
                if(splotchdistance<colorSplotches[i].growth) {
                    //int thisvalue = colorSplotches[i].splotchvector[splotchdistance];
                    //lamp[j] = blend(spoke[i],splotchcolor,thisvalue);
                    lamp[j] = splotchcolor;
                }
            }
            
        }
        else {
            colorSplotches.erase(colorSplotches.begin()+i);
        }
    }
    //spoke = currentspoke;
    copyspokes();
}

void rotate() {
    curpatternlen = 180;
    for(int i =0;i<3;i++) {
        int thiscoloridx = (rotatecoloridx+i)%3;
        int nextcoloridx = (rotatecoloridx+i+1)%3;
        CRGB blendcolor = blend(rotateColorList[thiscoloridx],rotateColorList[nextcoloridx],rotateblendidx*8);
        for(int j=0;j<SPOKELEN;j++) {
            lamp[i*SPOKELEN+j] = blendcolor;
        }
    }
    copyspokes();
}

void cyclecolor() {
    curpatternlen = 180;
    int nextcoloridx = (cyclecolorcoloridx+1)%cyclecolorarraysize;
    CRGB blendcolor = blend(cyclecolorColorList[cyclecolorcoloridx],cyclecolorColorList[nextcoloridx],cyclecolorblendidx*4);
    for(int i =0;i<NUM_LEDS;i++) {
        lamp[i] = blendcolor;
    }
    copyspokes();
}


void barber() {
    curpatternlen=180;
    //int stripewidth = SPOKELEN/blendburstnumbands;
    for(int j = 0;j<3;j++) {
        for (int i=0;i<SPOKELEN;i++) {
            int thiscolorindex = ((int(floor(float((i - barberpixelidx))/float(barberstripewidth)))+barbercoloridx+1))%3;
            if((i-barberpixelidx)%barberstripewidth==0) {
                CRGB thiscolor = blend(barberColorList[(thiscolorindex+j)%3],barberColorList[(thiscolorindex-1+3+j)%3],barberblendidx*64);
                lamp[j*SPOKELEN+i] = thiscolor;
            } else {
                lamp[j*SPOKELEN+i]=barberColorList[(thiscolorindex+j)%3];
            }
           
        }
    }
    copyspokes();
    //curpatternlen = 180;
    /*CRGB currentcolorarray[3];
    for(int i = 0;i<3;i++) {
        currentcolorarray[0] = blend(barberColorList[(barbercoloridx+i+1)%3],barberColorList[(barbercoloridx+i)%3],barberblendidx*4);
    }
    for(int i = 0;i<3;i++) {
        for(int j = 0;j<SPOKELEN;j++) {
            //lamp[i*SPOKELEN+j] = currentcolorarray[(i+j)%3];
            lamp[i*SPOKELEN+j] = barberColorList[(barbercoloridx+j+i)%3];
        }
    }
    copyspokes();*/
}

void noise() {
    curpatternlen = 180;
    for(int i=0;i<NUM_LEDS;i++) {
        lamp[i] = CRGB(random8(0,255),random8(0,255),random8(0,255));
    }
    copyspokes();
}

void nofunc() {}

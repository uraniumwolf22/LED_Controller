#include <Adafruit_NeoPixel.h>


#define LED_PIN 9
#define LED_COUNT 115
#define NUMLEDS 120
#define BRIGHTNESS 50

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

bool do_ = true;    //to make the main program only loop once and store the state
int colormin = 0;   //the minimum color
float colormax = 3000;//maximum color
int colors[NUMLEDS][3];

int positions[NUMLEDS][2] = {{493,215},{480,288},{467,362},{455,435},{442,508},{429,582},{416,655},{403,728},
                                  {390,801},{378,875},{365,948},{352,1021},{339,1095},{326,1168},{313,1241},{301,1315},
                                  {288,1388},{275,1461},{262,1535},{249,1608},{236,1681},{224,1754},{211,1828},{198,1901},
                                  {185,1974},{172,2048},{159,2121},{147,2194},{134,2268},{121,2341},{163,2403},{205,2465},
                                  {247,2527},{289,2590},{331,2652},{373,2714},{415,2776},{457,2838},{500,2900},{466,2819},
                                  {432,2738},{398,2657},{364,2576},{330,2495},{297,2414},{263,2333},{229,2252},{195,2171},
                                  {284,2297},{304,2228},{323,2159},{343,2090},{363,2021},{382,1952},{402,1884},{421,1815},
                                  {441,1746},{460,1677},{480,1608},{500,1539},{519,1608},{539,1677},{558,1746},{578,1815},
                                  {597,1884},{617,1952},{636,2021},{656,2090},{676,2159},{695,2228},{715,2297},{804,2171},
                                  {772,2249},{741,2326},{710,2403},{679,2480},{647,2558},{616,2635},{585,2712},{554,2789},
                                  {522,2867},{562,2808},{601,2750},{641,2691},{680,2633},{720,2575},{759,2516},{799,2458},
                                  {839,2399},{878,2341},{866,2270},{853,2199},{841,2128},{829,2056},{816,1985},{804,1914},
                                  {791,1843},{779,1772},{767,1701},{754,1630},{742,1559},{730,1487},{717,1416},{705,1345},
                                  {693,1274},{680,1203},{668,1132},{655,1061},{643,990},{631,919},{618,847},{606,776},{594,705},
                                  {581,634},{569,563},{556,492},{544,421},{532,350},{520,280},{507,210}};

void setup() {
  Serial.begin(9600);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);
  randomSeed(analogRead(0));
}

float linear_to_srgb_float(float x) {
    if (x <= 0.0f)
        return 0.0f;
    else if (x >= 1.0f)
        return 1.0f;
    else if (x < 0.0031308f)
        return x * 12.92f;
    else
        return powf(x, 1.0f / 2.4f) * 1.055f - 0.055f;
}

float srgb_to_linear_float(float x) {
    if (x <= 0.0f)
        return 0.0f;
    else if (x >= 1.0f)
        return 1.0f;
    else if (x < 0.04045f)
        return x / 12.92f;
    else
        return powf((x + 0.055f) / 1.055f, 2.4f);
}

int * compand(int rgb_int[3], bool inverse){    //calculate the compand
float rgb[3];

  for(int i = 0; i < 3; i++){
    rgb[i] = (float)rgb_int[i];
  }

  static int result[3];                       //define array to store final result
  for(int c = 0; c < 3; c++){                 //iterate over colors
    rgb[c] = rgb[c] / 255;                    //normalize?
    if(inverse){                              //choose if its an inverse compand
      srgb_to_linear_float(rgb[c]);}
      
    else{                                     //if not inverse
      linear_to_srgb_float(rgb[c]);}

    result[c] = rgb[c] * 255; //restore to original data size
   }

    return result;            //return the result of the compand
}

void staticGradient(int grcolor1[3],int grcolor2[3]){
  int * color1_;       //define pointer for color1 array
  static int color1[3];      //real color1 array
  int * color2_;       //define pointer for color2 array
  static int color2[3];      //real color2 array

  int * resultfinal;      //init the final result variable
  float factor;           //init the factor variable
  int res[3];           //init the result variable

  color1_ = compand(grcolor1,true);   //calculate compand for color1

  for(int i = 0; i < 3; i++){       //convert pointer to object
  color1[i] = *(color1_ + i);
  }

  color2_ = compand(grcolor2,true);   //calculate compand for color2
  
  for(int i = 0; i < 3; i++){       //convert pointer to object
    color2[i] = *(color2_ + i);
    }

  for(int i = 0; i < 3; i++){       //convert pointer to object
    color2[i] = *(color2_ + i);
    }

  for(int iterleds = 0; iterleds < NUMLEDS; iterleds++){  //iterate through list of LEDs
    factor = positions[iterleds][1] / colormax;           //calculate the factor

    for(int colorpos = 0; colorpos < 3; colorpos++){      //iterate through,  calculating LED values

      res[colorpos] = color1[colorpos] * (1 - factor) + color2[colorpos] * factor;  //calculate result
    }

    resultfinal = compand(res,false);                     //recalculate the final result

    for(int i = 0; i < 3; i++){
      colors[iterleds][i] = *(resultfinal + i);           //add results to the color list
      }
    }
  }


void loop() {
  int gradstart[3] = {255,0,0};  //define gradient start
  int gradend[3] = {0,255,0};    //define gradient end

  staticGradient(gradstart,gradend); //generate static gradient

  for(int led = 0; led < NUMLEDS; led++){

    strip.setPixelColor(led, strip.Color(colors[led][0],colors[led][1],colors[led][2]));
  }
  strip.show();
  delay(1000);

}
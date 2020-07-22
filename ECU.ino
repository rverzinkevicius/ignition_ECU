#include <ESP8266WiFi.h>


int sparkpin= D1;

int advance = 0;
unsigned long pickup=0;
int dwell_duration=3500;


int rpmpin = D2;
int rpm = 1;
float rpmai = 1;
unsigned long elapsedt_rpm = 1;
unsigned long last_update_rpm=0;
bool rpmflag=true;
bool rpmupdated=false;
unsigned long duration_rpmTmp=0;
volatile unsigned long duration_rpm=0;
volatile unsigned long last_rpm=0;



void setup()   {   


  WiFi.mode( WIFI_OFF );
  WiFi.forceSleepBegin();
  delay(300);

 Serial.begin(115200);

  pinMode(button,INPUT);
  pinMode(rpmpin,INPUT);


  attachInterrupt(digitalPinToInterrupt(rpmpin), rpm_counter, RISING); 



}

void loop() 
 {

 

if ((millis()-last_update_rpm) >100)
{
rpmflag=true;
}

 

if (rpmupdated){
  
  noInterrupts();
  rpmupdated=false;
  duration_rpmTmp=duration_rpm;
  pickup=last_rpm;
  interrupts();
  
 rpmai = 60000000/duration_rpmTmp;
 rpm = round (rpmai);


// below is ignition advance degree calculation

if (rpm<1000) {advance = 10;}
if ((rpm>=1000) && (rpm<1300)) {advance = 17;}
if ((rpm>=1300) && (rpm<1500)) {advance = 30-RPM/100;}
if ((rpm>=1500) && (rpm<3000)) {advance = 15;}
if ((rpm>=3000) && (rpm<4000)) {advance = 0.013*RPM-24;}
if ((rpm>=4000) && (rpm<7000)) {advance = 28;}
if ((rpm>=7000) && (rpm<8000)) {advance = 0.002*RPM+14;}
if (rpm>=8000) {advance = 30;}


spark=duration_rpmTmp*(63-advance)/360;   // 63 (or 60?) is degrees BTDC of pickup signal for Piaggio LEADER engine

dwell_start=duration_rpmTmp + spark - dwell_duration;

if (dwell_start>=duration_rpmTmp) {dwell_start-=duration_rpmTmp;} 


  Serial.print(rpm);
  Serial.print(",");
  Serial.print(spark);
  Serial.print(",");
  Serial.println(dwell_start);

last_update_rpm=millis();

}



if (((micros()-pickup)>=dwell_start) && (!rpmflag)) {digitalwirte(sparkpin,HIGH);}
if ((micros()-pickup)>=spark) {digitalwirte(sparkpin,LOW);}





yield(); 
}



ICACHE_RAM_ATTR void rpm_counter()   
{
if ((micros()-last_rpm)>5900)         //debounce signal. Take MAX RPM of engine and calculate time in micro seconds for full revolution: 
 {                                    //   60 000 000/(max_RPM+couple_hundred_for_error)  ->  60 000 000 / (10 000 + 200) = 5882 -> rounded it to 5900                             
  duration_rpm = micros()-last_rpm;
  last_rpm = micros();
  rpmupdated=true;
 }
}




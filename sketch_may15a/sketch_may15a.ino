



#include "DHT.h"


#include <DS3231.h> //Library for RTC module (Download from Link in article)
#include <SPI.h> //Library for SPI communication (Pre-Loaded into Arduino)
#include <SD.h> //Library for SD card (Pre-Loaded into Arduino)

#define DHT11_PIN 7 //Sensor output pin is connected to pin 7
#define DHTTYPE DHT11
DHT DHT(DHT11_PIN,DHTTYPE);
const int chipSelect = 4; //SD card CS pin connected to pin 4 of Arduino

// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);
float temp;
float humid;
void setup()
{
 
  // Setup Serial connection
  Serial.begin(9600);
  Initialize_SDcard();
  Initialize_RTC();
  Initialize_PlxDaq();
}

void loop()
{
 
  Read_DHT11();
  Read_TempHum();
  Write_SDcard();
  Write_PlxDaq();
  delay(5000); //Wait for 5 seconds before writing the next data 
}

void Write_PlxDaq()
  {
    Serial.print("DATA"); //always write "DATA" to Indicate the following as Data
    Serial.print(","); //Move to next column using a ","

    Serial.print("DATE"); //Store date on Excel
    Serial.print(","); //Move to next column using a ","

    Serial.print("TIME"); //Store date on Excel
    Serial.print(","); //Move to next column using a ","

    Serial.print(temp); //Store date on Excel
    Serial.print(","); //Move to next column using a ","

    Serial.print(humid); //Store date on Excel
    Serial.print(","); //Move to next column using a ","

    Serial.println(); //End of Row move to next row
  }

void Initialize_PlxDaq()
{
Serial.println("CLEARDATA"); //clears up any data left from previous projects
Serial.println("LABEL,Date,Time,Temperature,Humidity"); //always write LABEL, to indicate it as first line
}

void Write_SDcard()
{
    // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("LoggerCD.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(rtc.getDateStr()); //Store date on SD card
    dataFile.print(","); //Move to next column using a ","

    dataFile.print(rtc.getTimeStr()); //Store date on SD card
    dataFile.print(","); //Move to next column using a ","

    dataFile.print(temp); //Store date on SD card
    dataFile.print(","); //Move to next column using a ","

    dataFile.print(humid); //Store date on SD card
    dataFile.print(","); //Move to next column using a ","

    dataFile.println(); //End of Row move to next row
    dataFile.close(); //Close the file
  }
  else
  Serial.println("OOPS!! SD card writing failed");
}

void Initialize_SDcard()
{
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
   // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("LoggerCD.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println("Date,Time,Temperature,Humidity"); //Write the first row of the excel file
    dataFile.close();
  }
}

void Initialize_RTC()
{
   // Initialize the rtc object
  rtc.begin();

//#### The following lines can be uncommented to set the date and time for the first time###  

rtc.setDOW(FRIDAY);     // Set Day-of-Week to SUNDAY
rtc.setTime(18, 46, 45);     // Set the time to 12:00:00 (24hr format)
rtc.setDate(6, 30, 2017);   // Set the date to January 1st, 2014 
}

void Read_DHT11()
{
int chk = DHT.read(DHT11_PIN);
}

void Read_DateTime()
{  
  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");

  // Send time
  Serial.println(rtc.getTimeStr());
}

void Read_TempHum()
{
  Serial.print("Temperature = ");
  float t=DHT.readTemperature();
  temp=t;
  Serial.println(t);
  Serial.print("Humidity = ");
  float h=DHT.readHumidity();
  humid=h;
  Serial.println(h);
 delay(1000);
}

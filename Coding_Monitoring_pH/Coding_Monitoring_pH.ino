 

#include &amp;lt;ESP8266WiFi.h&amp;gt;
#include &amp;lt;WiFiClient.h&amp;gt;
#include &amp;lt;ThingSpeak.h&amp;gt;
#include &amp;lt;LiquidCrystal_I2C.h&amp;gt; //library untuk I2C LCD
LiquidCrystal_I2C lcd(0x27,16,2);  //alamat I2C dan ukuran LCD

#define pinPH D3  //pin output Sensor PH ditempatkan di D3
#define pinSoil D4 //pin output Sensor SoilMoisture ditempatkan di D4
int bacaSensorPH = 0;   //membaca hasil dari sensor pH
int bacaSensorSoil = 0; //membaca hasil dari sensor soilmoisture    
float nilaiPH = 0.0; //nilai pH yang ditampilkan
float nilaiSoil = 0.0;    //nilai kelembaban yang ditampilkan

const char *ssid = "*********";  
const char *password = "***********"; 
WiFiClient client; 

unsigned long myChannelNumber =  0000000; 
const char * myWriteAPIKey = "***************";

void setup() {
  lcd.init();      // mengawali LCD
  lcd.backlight(); // menyalakan backlight LCD
  pinMode(pinPH,INPUT);  //inisialisasi pinPH sebagai input
  pinMode(pinSoil,INPUT); //inisialisasi pinSoil sebagai input
  Serial.begin(115200); 
  
  Serial.print("Connecting to ");  
  Serial.println(ssid);  
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED)
  {
      delay(500);
      Serial.print(".");
  }
  Serial.println("");  
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client); 
}

void loop()   
{  
  bacaSensorPH = analogRead(pinPH); //baca pH
  bacaSensorSoil = analogRead(pinSoil); //baca Soil Moisture
 delay(500);
  nilaiPH = (-0.0139*bacaSensorPH)+7.7851; //rumus pembacaan sensor pH
  nilaiSoil = map(bacaSensorSoil, 1023, 465, 0, 100); //pengubahan nilai baca menjadi persen

  //tampilan LCD
  lcd.setCursor(0,0);
  lcd.print("pH = ");
  lcd.print(nilaiPH);
  lcd.setCursor(0,1);
  lcd.print("Lembab = ");
  lcd.print(nilaiSoil);
  
//Pengiriman ke thingspeak
  ThingSpeak.writeField(myChannelNumber, 1, nilaiPH, myWriteAPIKey); 
  ThingSpeak.writeField(myChannelNumber, 2, nilaiSoil, myWriteAPIKey);
  delay(2000);
}

// Include the libraries
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

#define ssid "STUDBME2"
#define pass "BME2Stud"

String saved_networks[] = {"StudBME1", "STUDBME2", "SBME_STAFF3", "SBME_STAFF", "CUFE", "RehabLab", "lab001", "CMP_LAB1"};
//String saved_networks[] = {"ahmed", "Imad", "Vodafone", "hp280", "OrangeDSL-BrainsOut", "Alwakiel", "mostafarefat", "Heba"};
String scanned_ssids[8];
int rssi_values[8];

int w_len = sizeof(saved_networks)/sizeof(saved_networks[0]);
int s_len = sizeof(scanned_ssids)/sizeof(scanned_ssids[0]);
int s_index = 0;          // index for scanned_ssids
int w_index = 0;          // index for saved_networks
int n = 0;                
int numberOfNetworks = 0; // number of scanned networks

void connect_to_WiFi();
void scan_networks();
void save_values();
void postreq (int arr[]);

void setup()
{
  Serial.begin(9600);
  Serial.println();
  connect_to_WiFi();
}
 
void loop(){
    scan_networks();
    delay(10000);  //Send a request every 10 seconds
    Serial.print("Data: "); 
    save_values();
}
 
//function for scanning networks and conncting to one of them
void connect_to_WiFi() {
  
   WiFi.begin(ssid, pass);
   Serial.println(); 
   Serial.println("connecting");
   
   while (WiFi.status() != WL_CONNECTED)
   {
    Serial.print("..");
    delay(500); 
   }
    Serial.println();
    Serial.print("connected to:");
    Serial.println(ssid);
    Serial.println("IP");
    Serial.println(WiFi.localIP());
} 
void scan_networks() 
{
  Serial.println("start scanning:-");
  int numberOfNetworks = WiFi.scanNetworks();
  if (numberOfNetworks == 0)
  {
      Serial.println("no networks found");
  }
  else
  {
   for(int i =0; i<numberOfNetworks; i++){
      Serial.print("Network name: ");Serial.println(WiFi.SSID(i));
      Serial.print("Signal strength: ");Serial.println(WiFi.RSSI(i));
      Serial.println("----------------");
     }
  }    
 }

void save_values(){
  int numberOfNetworks = WiFi.scanNetworks();
  // Save SSIDs and RSSIs to array
  for (int i = 0; i < numberOfNetworks; ++i)
  {
      // Check if the ssid exists in the saved networks
      s_index = findElement(saved_networks, w_len, WiFi.SSID(i));
      if (s_index != -1)
      {
          scanned_ssids[s_index] = WiFi.SSID(i);
          rssi_values[s_index] = WiFi.RSSI(i);
      } 
  }
  for (int i = 0; i < w_len; i++)
  {
      w_index = findElement(scanned_ssids, w_len, saved_networks[i]);
      // If it is saved network and not scanned -> put rssi = 0
      if (w_index == -1)
      {
          scanned_ssids[i] = saved_networks[i];
          rssi_values[i] = 0;
      }
  }
   // Send data to csv
  for (int i = 0; i < s_len; i++)
  {
      Serial.print(rssi_values[i]);
      Serial.print(",");
      delay(10);
  }

  Serial.println();
  postreq(rssi_values);
}
int findElement(String arr[], int n, String val)
{
    int indx = -1;

    for (int i = 0; i < n; i++)
    {
        // if found -> 0 -> !0 = 1 = True
        if(val == String(arr[i]))
        {
            indx = i;
            break;
        }
    }
    return indx;
}
// function to post values to server
void postreq (int arr[])
{
  // Set up the client objet
     WiFiClient client;
     HTTPClient http;
     
    http.begin(client,"http://toto3.pythonanywhere.com/Post_Readings/");   //Specify request destination
    http.addHeader("Content-Type",  "application/json");  //Specify content-type header

      char json_str[100];
      sprintf(json_str, "{\"Readings\":[%d,%d,%d,%d,%d,%d,%d,%d]}", rssi_values[0], rssi_values[1], rssi_values[2], rssi_values[3],
                                                     rssi_values[4], rssi_values[5], rssi_values[6], rssi_values[7]);
      int httpCode = http.POST(json_str);
      Serial.println("in post fun");
//    
    if(httpCode > 0){
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          Serial.print("Response: ");
          Serial.println(httpCode);
          String payload = http.getString();
          Serial.println(payload);
        }
    }
    else{
         Serial.printf("[HTTP] GET... failed, error: %s", http.errorToString(httpCode).c_str());
    }
        http.end();  //Close connection
    
}

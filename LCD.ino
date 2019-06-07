
#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266HTTPClient.h>  // Include this library, which provides methods to send HTTP requests.
#include <ArduinoJson.h>        //provides the ability to parse and construct JSON objects         
#include <PubSubClient.h>   
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <SPI.h>


int current = 0;
int last = -1;
#define PHOTOCELL_PIN A0

//#define wifi_ssid "NETGEAR12"
//#define wifi_password "Deansucks1992"

#define wifi_ssid "University of Washington"  
#define wifi_password "" 

#define mqtt_server "mediatedspaces.net"  //this is its address, unique to the server
#define mqtt_user "hcdeiot"               //this is its server login, unique to the server
#define mqtt_password "esp8266" 

String noun[] = { "police", "Kanye", "gardner", "Alaa", "You", "She", "Dan"," I", "Hannah", "Brock", "Steven", "fish", "animals", "men", "women", "fro gs", "kids","humans", "idiots", "pewdiepie"
};

String verb[] = { 
  "become", "begin", "eat", "sleep","shower", "run","treat", "view", "clap", "hit", "make", "grow", "swim", "scare", "love", "hate", "lend", "write", "ride", "adpat", "arise" 
};
 
String object[] = { "shampoo", "iPhone", "book", "shoe", "sneaker", "bag", "needle", "knife", "gun", "tissue", "camera", "umbrella", "headphone", "pen", "notebook", "rubbish", "painkiller" 
  
};


WiFiClient espClient;             
PubSubClient mqtt(espClient); 

char mac[6]; //A MAC address is a 'truly' unique ID for each device, lets use that as our 'truly' unique user ID!!!
char message[201];
char x[200];

typedef struct { // here we create a new data type definition, a box to hold other data types
  String check;
} Trump;     //then we give our new data structure a name so we can use it in our code

Trump ishe;        //we have created a Quote type, but not an instance of that type,
                   //so we create the variable 'said' of type Quote

                   
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Start the Serial communication to send messages to the computer
  setup_wifi();
  mqtt.setServer(mqtt_server, 1883);
  mqtt.setCallback(callback);
  //Serial.println(ishe.check);
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqtt.connected()) {
    //Serial.print("Attempting MQTT connection...");
    if (mqtt.connect(mac, mqtt_user, mqtt_password)) { //<<---using MAC as client ID, always unique!!!
      //Serial.println("connected");
      mqtt.subscribe("Alaa/+");     //we are subscribing to 'Alaa' and all subtopics below that topic
    } else {                        //please change 'theTopic' to reflect your topic you are subscribing to
      //Serial.print("failed, rc=");
      //Serial.print(mqtt.state());
      //Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  current = analogRead(PHOTOCELL_PIN);
  if (current < 100) {
    sentin();
    delay(2000);
  }
  current = map(current, 0, 1023, 0, 255);

 if(current == last)
    return;
 Serial.println(current);
 last = current;
 delay(100);

 if (!mqtt.connected()) {
    reconnect();
  }
  mqtt.loop();

}
void setup_wifi() {
  delay(10);
  // Start by connecting to a WiFi network
  //Serial.println();
  //Serial.print("Connecting to ");
  //Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //Serial.println("");
  //Serial.println("WiFi connected.");  //get the unique MAC address to use as MQTT client ID, a 'truly' unique ID.
  //Serial.println(WiFi.macAddress());  //.macAddress returns a byte array 6 bytes representing the MAC address
  
} 

void sentin() {   // function called dumbQuote that provides Donald Trump Quotes
  HTTPClient theClient;  // Use HttpClient object to send requests
  String apiCall = "http://lt-nlgservice.herokuapp.com/rest/english/realise?subject=";
  apiCall += noun[random(18)];
  apiCall += "&verb=";
  apiCall += verb[random(20)];
  apiCall += "&object=";
  apiCall += object[random(16)];
  
  //apiCall += =man&verb=play&object=thing
  theClient.begin(apiCall); // API call with search query attached
  int httpCode = theClient.GET();
  if (httpCode > 0) { // if we get something back
    if (httpCode == HTTP_CODE_OK) {
      String payload = theClient.getString();
      DynamicJsonBuffer jsonBuffer; //  Dynamic Json buffer is allocated on the heap and grows automaticallyis 
      // it is also the entry point for using the library: it handles the memory management and calls the parser
      JsonObject& root = jsonBuffer.parseObject(payload);
      
      if (!root.success()) { // Test if parsing succeeds.
        Serial.println("parseObject() failed in getMet()."); // if parsing doesn't successed, print that to serial monitor 
        return;
      }
       ishe.check = root["sentence"].as<String>();
        if (ishe.check.length() > 2) {
          String alaa = ishe.check;
          alaa.toCharArray(x, (alaa.length()));
          sprintf(message, "{\"sentence\":\"%s%\"}", x); 

          mqtt.publish("Alaa/sent", message); 
  }
       
    }
  }else {
    Serial.println("Something went wrong with connecting to the endpoint in getMet()."); // if we were not, for some reason, able to receive responses, then print this tp dserial monitor 
  }
}

void callback(char* topic, byte* payload, unsigned int length) {


  DynamicJsonBuffer  jsonBuffer; //blah blah blah a DJB
  JsonObject& root = jsonBuffer.parseObject(payload); //parse it!

  if (!root.success()) { //well?
    Serial.println("parseObject() failed, are you sure this message is JSON formatted.");
    return;
  }

  //root.printTo(Serial); //print out the parsed message
  //Serial.println(); //give us some space on the serial monitor read out
}

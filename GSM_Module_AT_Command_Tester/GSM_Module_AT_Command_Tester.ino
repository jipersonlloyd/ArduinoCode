#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2);  // RX, TX
String data;
String mcc;
String mnc;
String cellID;
String lac;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  mySerial.begin(9600);
  mySerial.println("AT");
  delay(1000);
  mySerial.println("AT+CGATT=1");
  delay(1000);
  mySerial.println("AT+CENG=1,1");
  mySerial.println("AT+CENG?");
}

void loop()  // run over and over
{
  if (mySerial.available())
    data = mySerial.readStringUntil('\n');
  // Serial.write(mySerial.read());
  if (data.startsWith("+CENG: 0")) {
    mcc = data.substring(21, 24);
    mnc = data.substring(26, 27);
    cellID = "cb7c";
    // cellID = data.substring(31, 35);
    lac = data.substring(42, 46);
    Serial.print("MCC: ");
    Serial.println(mcc);
    Serial.print("MNC: ");
    Serial.println(mnc);
    Serial.print("cellID: ");
    Serial.println(cellID);
    Serial.print("lac: ");
    Serial.println(lac);
    // if (sendPostRequest()) {
    //   Serial.println("POST request sent successfully");
    //   delay(10000);
    // } else {
    //   Serial.println("Failed to send POST request");
    //   delay(10000);
    // }
    return;
  }

  if (Serial.available()) {
    while (Serial.available()) {
      mySerial.write(Serial.read());
    }
    mySerial.println();
  }
}


bool sendPostRequest() {
  String url = "http://jipersonlloyd-001-site1.etempurl.com/api/cellTowerData/data?cellID=" + cellID + "&lac=" + lac + "&mcc=" + mcc + "&mnc=" + mnc;
  Serial.println(url);
  mySerial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  delay(1000);
  mySerial.println("AT+SAPBR=3,1,\"APN\",\"internet\"");
  delay(1000);
  mySerial.println("AT+SAPBR=1,1");
  delay(2000);

  // Replace the following values with your actual API endpoint and payload
  mySerial.println("AT+HTTPINIT");
  delay(2000);
  mySerial.print("AT+HTTPPARA=\"URL\",\"");
  mySerial.println(url);
  delay(2000);
  mySerial.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  delay(2000);
  mySerial.println("AT+HTTPDATA=1000,10000");
  delay(2000);
  mySerial.println("{\"key\":\"value\"}");
  delay(2000);
  mySerial.println("AT+HTTPACTION=1");
  delay(60000);
  mySerial.println("AT+HTTPREAD");
  delay(2000);

  String response;
  while (mySerial.available()) {
    char c = mySerial.read();
    response += c;
  }

  Serial.println(response);

  if (response.indexOf("HTTP/1.1 200 OK") != -1) {
    return true;
  } else {
    return false;
  }
}
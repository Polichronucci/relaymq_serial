/*
  Copyright (C) 2018  Polichronucci

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define RELAY1  52
#define RELAY2  22

void setup()
{
  // Initialize the Arduino data pins for OUTPUT
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  
//setting all pins to low so that all relays are off at start
  digitalWrite(RELAY1,LOW);
  digitalWrite(RELAY2,LOW);
  
  //wait for serial connect from COM Port
  while (!Serial);
  Serial.begin(19200);

  Serial.setTimeout(5000);
}

void loop()
{
  String command;
  char charBuf[10];
  int r1,r2;
  
  //waiting for data command
  if(Serial.available() > 0)
  {
    command = Serial.readStringUntil('\r');
    command.toCharArray(charBuf, 10);
  }
  else
    return;

  //-1     - off relay 01 
  //+1     - on relay 01
  //1      - toggle relay 01
  //status - show status of relays
  
  if  (charBuf[0]=='+') //relay on
    SetStatus(true, charBuf[1]);
  else if (charBuf[0]=='-') //relay off
    SetStatus(false, charBuf[1]);
  else if (charBuf[0]=='1')  //toggle relay 1
  {
    if (digitalRead(RELAY1))
      SetStatus(false, charBuf[0]);
    else
      SetStatus(true, charBuf[0]);
  }
  else if (charBuf[0]=='2')  //toggle relay 1
  {
    if (digitalRead(RELAY2))
      SetStatus(false, charBuf[0]);
    else
      SetStatus(true, charBuf[0]);
  }
  else if (strstr(charBuf, "status"))
  {
    r1=digitalRead(RELAY1);
    r2=digitalRead(RELAY2);
    Serial.print(r1);
    Serial.print(" ");
    Serial.println(r2);
  }
}

void SetStatus(boolean on,char relayNumber)
{
  int relayState =0;//off 
  if (on)
    relayState =1;//on 

  switch (relayNumber) {
  case '1':
    digitalWrite(RELAY1,relayState);
    break;
  case '2':
    digitalWrite(RELAY2,relayState);
    break;
  }

  delay(1000*15);

  while(Serial.available())
    Serial.read();
}

void displayGraph(){
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW,TFT_BLACK);
  tft.setTextSize(1);

  int dwindow = dmax - dmin;
  
  if(dwindow < 100){
    dwindow = 100;
  }

  int dbot = dmin - (dwindow / 2);
  int dtop = dmax + (dwindow / 2);
  int dteil = (dtop - dbot) / 6;
  
  tft.setCursor(0, 120, 2);
  tft.printf("%02d", dbot / 10);  
  tft.setCursor(0, 98, 2);
  tft.printf("%02d", (dbot + dteil) / 10); 
  tft.setCursor(0, 75, 2);
  tft.printf("%02d", (dbot + (dteil * 2)) / 10); 
  tft.setCursor(0, 52, 2);
  tft.printf("%02d", (dbot + (dteil * 3)) / 10); 
  tft.setCursor(0, 29, 2);
  tft.printf("%02d", (dbot + (dteil * 4)) / 10); 
  tft.setCursor(0, 4, 2);
  tft.printf("%02d", dtop / 10);
  
  tft.drawLine(16,134,16,0, TFT_YELLOW);
  tft.drawLine(16,134,239,134, TFT_YELLOW);

  int p = 0;
  int x = 0;
  int x1 = 0;
  int y = 0;
  int d = 1;
  int t = 0;
  int t1 = 0;

  tempdata = "\"Punkt\",\"" + sensor1 + "\"\n";
  
  if(werte < 223){
    p = 0;
  } else {
    p = pos;
  }

  for(int i = 0; i < werte; i++){
    
    t = ringbuffer[p];
    
    p++;
    if(p >= 224){
      p = 0;    
    }
    
    x = map(t,dbot, dtop, 134, 0);
    tft.drawPixel(17 + y, x, colors[color]);
    tempdata += String(d++) + "," + String(t / 100.0) + "\n";
    y ++;    
  }
  //Serial.println(tempdata);
}

void displayData(){  
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW,TFT_BLACK);  
  tft.setTextSize(2);
  tft.setCursor(0, 0, 2);
  tft.println(sensor1);
  tft.setTextSize(1); 
  tft.setCursor(160, 0, 2);
  //tft.println(voltage);
  
  tft.setTextSize(2);
  int xpos = tft.width() / 2; // Half the screen width
  int ypos = tft.height() - 5;
  
  tft.setTextColor(colors[color], TFT_BLACK, true);
  tft.setTextDatum(C_BASELINE);
  tft.drawString(String(ringbufferGet() / 100.0), xpos, ypos, 6);
}

void displayWifi(){
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW,TFT_BLACK);  
  tft.setTextSize(2);
  tft.setCursor(0, 0, 2);
  if(config_done){
    tft.println("Wifi:");
    tft.println(ssid); 
    tft.setTextSize(1);
    tft.print("IP: ");
    tft.println(WiFi.localIP()); 
  } else {
    tft.println("WifiAP:");
    tft.println(apssid);
    tft.setTextSize(1);
    IPAddress myIP = WiFi.softAPIP();
    tft.print("IP: ");
    tft.println(myIP);     
  }
}
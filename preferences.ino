void prefClear(){
  pref.begin("config", false);
  pref.clear();
  pref.end();
}

void prefRead(){
  pref.begin("config", false);
  apssid = pref.getString("apssid", "Wifi_Drucksensor");
  appassword = pref.getString("appass", "12345678");
  ssid = pref.getString("ssid");
  password = pref.getString("pass");
  sensor1 = pref.getString("s1", "System-Druck");
  color = pref.getInt("col", 0);
  storeinterval = pref.getInt("store", 1);
  config_done = pref.getInt("done", 0); 
  mqttServer = pref.getString("mqttServer", "");
  mqttUser = pref.getString("mqttUser", "");
  mqttPass = pref.getString("mqttPass", "");
  adc_zero = pref.getInt("zero", 150);         //ADC Wert - 0 BAR
  adc_value = pref.getInt("aval", 1085);       //ADC Wert - 2 BAR
  presure_value = pref.getInt("pval", 200);    //Druck Wert * 100 bei ADC Wert (5 BAR -> 500)   
  pref.end();
  yield();
}

void prefWrite(){
  pref.begin("config", false);
  pref.putString("apssid", apssid);
  pref.putString("appass", appassword);
  pref.putString("ssid", ssid);
  pref.putString("pass", password);
  pref.putString("s1", sensor1);
  pref.putInt("col", color);
  pref.putInt("store", storeinterval);
  if(ssid.length() < 4 || password.length() < 8){
    pref.putInt("done", 0);
  } else {
    pref.putInt("done", 1);
  }
  pref.putString("mqttServer", mqttServer);
  pref.putString("mqttUser", mqttUser);
  pref.putString("mqttPass", mqttPass);
  pref.putInt("zero", adc_zero);
  pref.putInt("aval", adc_value);
  pref.putInt("pval", presure_value);
  pref.end();
}

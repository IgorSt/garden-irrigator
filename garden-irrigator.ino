#include <Wire.h> 
#include <DS1307ESP.h>

#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 3600 // 3600 seg = 1h
#define TIME_TO_SLEEP2h 7200 // 7200 seg = 2h
#define TIME_TO_SLEEP20h 72000 // 72000 seg = 20h

DS1307ESP rtc;

int RELE1 = 19;
int RELE2 = 18;

int start_operation = 19;

void setup() {
  Serial.begin(9600);
  Serial.println("");
  Serial.println("ACORDOU");

  rtc.begin();

  //rtc.DSadjust(22, 40, 00, 2023, 4, 27); // 00:19:21 16 Mar 2022

  rtc.DSread();
  
  pinMode(RELE1, OUTPUT);
  pinMode(RELE2, OUTPUT);

  boolean isDay = rtc.dayOfWeek == 1 || rtc.dayOfWeek == 3 || rtc.dayOfWeek == 5 || rtc.dayOfWeek == 0; // Segunda(1), Quarta(3) e Sexta(5) e Domingo(0)
  if (rtc.hour >= start_operation && isDay) {
    // ACIONA VÁLVULA
    open();

    delay(7200000); // 2HORAS

    // DESLIGA VÁLVULA
    close();

    Serial.println("SETA 20h PARA ACORDAR");
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP20h * uS_TO_S_FACTOR);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
    Serial.println("COMECA DORMIR");
    esp_deep_sleep_start();
  } else {
    // DESLIGA VÁLVULA
    close();

    showTime();

    Serial.println("SETA 1 HORA PARA ACORDAR");
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
    Serial.println("COMECA DORMIR");
    esp_deep_sleep_start();
  }
}

void loop() {
}

void showTime() {
  rtc.DSread(); 
  
  Serial.println(rtc.getDateTime(true));  //  (String) Monday, 14-March-2022 00:09:21
  Serial.println(rtc.dayOfWeek);          //  (int)     1     (0-6)
  Serial.println(""); 
}

void open() {
  digitalWrite(RELE1, LOW);
  digitalWrite(RELE2, HIGH);
  delay(12000);
  digitalWrite(RELE1, HIGH);
  Serial.println("VALVULA ABERTA");
}

void close() {
  digitalWrite(RELE1, HIGH);
  digitalWrite(RELE2, LOW);
  delay(12000);
  digitalWrite(RELE2, HIGH);
  Serial.println("VALVULA FECHADA");
}

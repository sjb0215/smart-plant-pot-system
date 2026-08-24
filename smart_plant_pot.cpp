#include <LiquidCrystal_I2C.h>

// LCD I2C 주소 및 규격 설정 (0x27, 16열 2행)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 입출력 핀 번호 지정
int sensor = A0;
int IN1 = 2;
int IN2 = 3;

void setup() {
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
    
    // 각 입출력 핀 선언
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
}

void loop() {
    int value = analogRead(sensor);

    // 센서 아날로그 입력 보정 (Min/Max 클램핑)
    if (value < 900) value = 900;       // 물이 많을 때의 값 (Min 처리)
    if (value > 1010) value = 1010;     // 물이 적을 때의 값 (Max 처리)

    // 0~100% 정규화 (1010 -> 0%, 900 -> 100%)
    int new_value = map(value, 1010, 900, 0, 100);

    Serial.print("new_value=");
    Serial.print(new_value);
    Serial.println(" %");

    // LCD 0번째 행에 수분 퍼센테이지 출력
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(new_value);
    lcd.print("%");

    // 수분 상태에 따른 워터펌프 모터 제어 및 LCD 알림
    if (new_value < 40) { // 습도 40% 미만 시 급수 동작
        lcd.setCursor(0, 1);
        lcd.print("water, please!");
        digitalWrite(IN1, HIGH);
    } else {              // 습도 40% 이상 시 급수 중단
        lcd.setCursor(0, 1);
        lcd.print("Enough!Moisture");
        digitalWrite(IN1, LOW);
    }

    delay(1000);
}
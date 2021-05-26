#define TX_PIN    12

#define T         1000    // длительность условного стандартного импульса
#define ZERO_DUR  T       // длительность "нуля"
#define ONE_DUR   T * 2   // длительность "единицы"

#define SIZE_OF_BYTE  8

void setup() {  
  pinMode(TX_PIN, INPUT_PULLUP); // ПИН ОБЯЗАТЕЛЬНО PULLUP
}

void loop() {
  byte data[] = {'H', 'E', 'L', 'L', 'O'};
    
  pinMode(TX_PIN, OUTPUT);
  for (byte bytes = 0; bytes < sizeof data; bytes++) {
    for (byte bits = 0; bits < SIZE_OF_BYTE; bits++) {
      bool bit = (data[bytes] >> bits) & 1;

      digitalWrite(TX_PIN, LOW);  // -----\_
      delayMicroseconds((bit ? ONE_DUR : ZERO_DUR));
      digitalWrite(TX_PIN, HIGH); // ____/-
      
      delayMicroseconds(T); // задержка между битами
    }
  }
  pinMode(TX_PIN, INPUT_PULLUP);
  
  delay(1000); // задержка между пакетами данных
}

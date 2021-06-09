/*
    Скетч приемника данных по одному проводу.

    Copyright (C) 2021 Alexey Silichenko (a.silichenko@gmail.com)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
    USA
 */

#define T 1000 // исходная длительность условного импульса

// ------\_/-
#define ZERO_L 500  // нижняя граница длительности импульса "нуля"
#define ZERO_H 1500 // верхняя граница длительности импульса "нуля"

// ------\____/-
#define ONE_L 1500 // нижняя граница длительности импульса "единицы"
#define ONE_H 2500 // верхняя граница длительности импульса "единицы"


#define F_CPU 1200000UL // ATTINY13

#define DATA_PIN  PB1 // INT0

#define DATA_SIZE       9
#define IS_BYTE_FULL(n) (7 == n)
#define IS_BUF_FULL(n)  ((DATA_SIZE - 1) == n)

byte buf[DATA_SIZE] = {0}, 
     data[DATA_SIZE] = {0};
     
volatile byte byteCount = 0, bitCount = 0;

void setup() {
  pinMode(DATA_PIN, INPUT_PULLUP); // чтобы не ловить помехи
  
  attachInterrupt(DATA_PIN, intHandler, CHANGE);
}

void loop() {
  
}

void flush() {
  byteCount = 0;
  bitCount = 0;
  memset(buf, 0, sizeof buf);
}

volatile long prevTime = -1;
void intHandler() {
  if (prevTime < 0) {
    prevTime = micros();
    return;
  }

  long dur = micros() - prevTime;
  prevTime = micros();
  
  if(LOW == digitalRead(DATA_PIN)) { // ----\__
    if(dur > 10 * T) flush();        // истек срок давности данных
  } else { // ___/-----
    bool bit;
    if(ZERO_L <= dur && dur < ZERO_H) bit = 0;
    else if(ONE_L <= dur && dur < ONE_H) bit = 1;
    else { // длительность импульса неопределена - ошибка приема, сбрасываем состояние
      flush();
      return;
    }
    
    bitWrite(buf[byteCount], bitCount, bit);
    
    if(IS_BUF_FULL(byteCount) && IS_BYTE_FULL(bitCount)) {  // передача данных окончена
      memcpy(data, buf, DISP_LEN);
      flush();
    } else if(IS_BYTE_FULL(bitCount)) { // очередной байт полностью передан
      bitCount=0;
      byteCount++;
    } else {
      bitCount++;
    }
  }
}

/*
    Скетч отправителя данных по одному проводу.

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

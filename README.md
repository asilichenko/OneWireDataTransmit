# OneWireDataTransmit
 
Простейший протокол передачи данных по одному проводу (плюс GND) без синхронизации.

Можно подключить ATTiny13 к Arduino и передавать какие-либо данные или команды с Arduino на ATTiny13. Можно и наоборот, в любом случае - это занимает лишь один пин. Важно только, чтобы используемый пин поддерживал аппаратные прерывания (INT0). На программных прерываниях (PCINT) - стабильную передачу данных установить не удалось.

Суть метода:
- "ноль" кодируется "коротким" импульсом
- "единица" - длинным
- базовое состояние линии передачи - высокий уровень сигнала
- импульсом считаеся время пока на линии низкий уровень сигнала
- при каждом изменении уровня сигнала срабатывает аппаратное прерывание на получателе
- получатель измеряет время между текущим и предыдущим прерываниями
- на время передачи данные хранятся в буфере
- как только пакет передан полностью - данные переносятся из буфера в основное хранилище для обработки

Как выглядят данные при передаче:

0:  ------\\_/----------

1:  ------\\__/---------

00: ------\\\_/-\\\_/------

01: ------\\_/-\\__/------

10: ------\\__/-\\_/------

11: ------\\\_\_/-\\\_\_/-----

# Комплект материалов для мастер-класса по IoT
Всё готовилось как материалы мастер-класса в рамках весенней образовательной школы [BEST UrFU 2019](https://urfu.ru/ru/students/leisure/oso/best/)

> Disclaimer: Авторы осознают, что использовать Wi-Fi для интернета вещей немного не... энергоэффективно.

## Требования

1. Быстро (6 астрономических часов) показать создание конечных устройств для интернета вещей
2. Минимальный порог вхождения и простота архитектуры для повторения 
3. Кроссплатформенность
4. Разумные затраты времени и денег

## Архитектура

1. Отладочные платы WEMOS D1 Mini (ESP 8266) подключаемые к сенсорам/актуаторам
2. Wi-Fi маршрутизатор, выпускающий контроллеры в интернет/локальную сеть
3. Сервер MQTT, который взаимодействует с контроллерами
4. Сервер Node-RED, который позволяет поверх MQTT реализовать дополнительную логику и предоставляет пользовательский интерфейс
5. MQTT клиент под Android (MQTT Dash, Linear MQTT Dashboard) или под IoS (MQTTool, MQTT Client) для демонстрации простоты ручного управления устройствами.

Учитывая, что надо было озадачить несколько команд по 2 студента разными задачами, требовалось показать совместную работу устройств в рамках интернета вещей, были выдвинуты следующие 

### Задачи

0. [Подготовка](https://github.com/plazmer/lc_iot/blob/master/instructions/00_1_setup_arduino_ide.md) рабочего [места](https://github.com/plazmer/lc_iot/blob/master/instructions/00_2_mqtt_basic.md) - настройка [среды](https://github.com/plazmer/lc_iot/blob/master/instructions/00_3_mqtt_work.md) Arduino IDE, [простой](https://github.com/plazmer/lc_iot/blob/master/instructions/00_4_WEMOS_pins_vs_Arduino_pins.md) код для тестирования работы (основан на примере [github knolleary](https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_esp8266/mqtt_esp8266.ino)) в 4 частях

1. [Датчик температуры и влажности на основе DHT11](../instructions/01_dht11.md) ([готовый код](../01_dht11))
2. [Аналоговый датчик освещенности на фоторезисторе](../02_light_analog.md) ([готовый код](../02_analog_light/))
3. [Цифровой датчик освещенности на фоторезисторе](https://github.com/plazmer/lc_iot/blob/master/instructions/03_light_digital.md) ([готовый код](https://github.com/plazmer/lc_iot/tree/master/03_digital_light))
4. [Умная RGB лампа (адресные светодиоды)](https://github.com/plazmer/lc_iot/blob/master/instructions/04_rgb_led.md)
5. [Датчик движения](<https://github.com/plazmer/lc_iot/blob/master/instructions/05_guard.md>)
6. [Умные жалюзи (сервопривод)](https://github.com/plazmer/lc_iot/blob/master/instructions/06_smart_blinds.md)
7. [Умные шторы (электродвигатель с редуктором, драйвер)](https://github.com/plazmer/lc_iot/blob/master/instructions/07_smart_curtains.md)
8. [Умная розетка (это было совсем скучно, поэтому тут пусто)](https://github.com/plazmer/lc_iot/blob/master/instructions/08_smart_outlet.md)
9. [Умный экран (светодиодная матрица на MAX7219)](<https://github.com/plazmer/lc_iot/blob/master/instructions/09_screen.md>)
10. RFID считыватель (планировался, но не удалось завести по SPI работу с ним, задача была заменена по месту на PWM регулирование освещённости)
11. [Настройка сервера (Ubuntu Server 18.04 LTS)](https://github.com/plazmer/lc_iot/blob/master/instructions/11_server_setup.md)
12. [Настройка Node-RED](https://github.com/plazmer/lc_iot/blob/master/instructions/12_node-red_setup.md))

Задания были подобраны таким образом, чтобы каждая команда могла взаимодействовать с кем-то еще для получения сложных сценариев поведения, при этом ограниченность возможностей локальной задачи должна была их поощрять к совместной работе.

Примеры:

(6) Открыть жалюзи в зависимости от (3) освещения

(4) Зажечь лампу при (5) приближении

Вывести текущую (1) погоду на (9) экран

(10) При открытии двери (7) закрыть шторы, если вечер и (4) зажечь свет

(8) включить чайник на 2 минуты по команде

(10) адаптивным освещением компенсировать (2) недостаток света

автоматическое (3) включение освещения в подъезде (5) в темное время суток при приближении человека

### Краткий бэклог

Микроконтроллер с 3.3 В логикой и 5 В на выходе отладочной платы иногда внезапно выпускает белый дым, нужен запас контроллеров или очень строгий контроль сборки и подключений.
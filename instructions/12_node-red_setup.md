# Настройка Node-RED

Дополнительные материалы 
https://nodered.org/docs/getting-started/

Основная единица работы с логикой приложения для Node-RED - flow. Он состоит из node (минимальная единица логики), которые можно собирать вместе в большую и сложную систему обработки событий.

Для входа в систему надо узнать у инструктора адрес сервера и зайти на URL:

http://INSERT_IP_ADDRESS_HERE:1880

## Общий вид

![](img/12/editor-default.png)

The editor window consists of four components:

The header at the top, containing the deploy button, main menu, and, if user authentication is enabled, the user menu.
The palette on the left, containing the nodes available to use.
The main workspace in the middle, where flows are created.
The sidebar on the right.

![](img/12/editor-default-components.png)

## Предварительная настройка

Сначала надо установить дополнение `node-red-dashboard`вызывает . В правом верхнем углу страницы кнопка ![](img/12/menu_button.png) вызывает меню.

![](img/12/menu_manage_palette.png)

Меню **Manage palette**, 

раздел по вертикали **Palette**, раздел по горизонтали **Install**

![](img/12/palette_install.png)

Ввести в поле поиска **node-red-dashboard,** нажать **Install**, снова **Install**

![](img/12/palette_install_dashboard.png)

![](img/12/palette_install_dashboard_install.png)

После завершения установки можно закрыть всплывающие окна (кнопка ![Close](img/12/palette_install_closel.png)) и перейти к настройке.

## Базовые ноды

Ноды в палитре группируются по разделам.

Самые важные для работы приложения разделы: 

- **Input** - поступление входящих сигналов 
- **Output** - отправляют сигналы в результате принятых решений
- **Function** - позволяют применить дополнительные преобразования к данным и принимать решения
- **Social** - отсылка/прием сообщений (в базовой поставке) с email и twitter, после установки дополнительных плагинов можно расширить возможности еще больше.
- **dashboard** - появляется после установки `node-red-dashboard`, содержит компоненты для постройки визуального интерфейса.

## First Flow

В рамках подготовки к работе настраивали работу контроллера с сервером по MQTT, теперь сделаем полный цикл настройки для работы с помощью Node-RED.

Первым делом надо добавить кнопку, которая будет зажигать светодиод.

Из палитры в разделе **dashboard** с помощью перетаскивания добавляем две ноды **button** ![](img/12/node_dashboard_button.png) - для включения и выключения, а из раздела **output** ноду **mqtt** ![](img/12/node_output_mqtt.png)

Потом соединить выходы (слева) нод button и вход (справа) ноды mqtt при помощи drag-drop мышкой.

![](img/12/flow_two_button_mqtt.png)

Двойным щелчком по верхней ноде **button** откроем диалог редактирования.

![](img/12/node_input_button_edit_before_ON.png)

1. Добавляем ui_group - группа колонок в будущем интерфейсе, в которой будет размещаться визуальный элемент, кнопкой ![](img/12/node_input_button_pencil.png).
   ![](img/12/node_input_button_add_dashboard_group.png)
2. Для группы надо аналогичным ![](img/12/node_input_button_pencil.png) образом добавить колонку (**Tab**)
   ![](img/12/node_input_button_add_dashboard_tab.png)
3. **Add**

В итоге диалог будет выглядеть так:

![](img/12/node_input_button_edit_after_ON.png)



Аналогично настроить нижнюю кнопку:

![](img/12/node_input_button_edit_after_OFF.png)



Для ноды MQTT потребуется добавить сервер. 

Параметры используйте те же, что и в config.h прошивки (уточните у инструктора)

```c++
#define mqtt_server     "2.remzalp.ru"
#define mqtt_port     9976
#define mqtt_login    "student"
#define mqtt_pass     "rtf-123"
```

Реквизиты тестового сервера уточните у инструктора, в дальнейшем будете использовать собственный сервер, который сейчас настраиваете)

![](img/12/node_output_mqtt_edit_before.png)

Аналогично потребуется добавить сервер **mqtt-broker**:

![](img/12/node_output_mqtt_server.png)

Параметры **General / Server, Port и Security / Username, Password** уточняйте у инструктора.

![](img/12/node_output_mqtt_edit_after.png)

Параметр **Topic** используйте тот же, что и в прошивке в параметре **mqtt_topic_in**.

Чтобы сразу посмотреть на результат, можно нажать кнопку **Deploy** ![](img/12/menu_deploy.png). Это сохраняет текущий Flow и применяет настройки на сервере.

![](img/12/flow_two_button_mqtt_connected.png)

Чтобы посмотреть результат в визуальном режиме - **Menu / View / Dashboard**

![](img/12/menu_view_dashboard.png)

А затем в **sidebar** на вкладке **Site** воспользоваться кнопкой ![](img/12/button_dashboard_out.png) для перехода на интерфейс. Заодно на этой же панели во вкладках вкладке можно настроить дополнительные параметры отображения.

![](img/12/menu_dashboard_site.png)

Вообще, хорошей идеей будет держать две вкладки открытыми одновременно - с редактором и графическим интерфейсом.

![](img/12/dashboard_two_buttons.png)

Можно видеть (в окне Putty), как при нажатии кнопок на MQTT сервер отправляется значение нажатой кнопки. Если контроллер включен и всё правильно настроено - время реакции на команду будет менее 1 секунды.

## Реализуем графику

Теперь дополним ноду для приёма сообщений - **input / mqtt** ![](img/12/node_input_mqtt.png)и ноду для вывода графики **dashboard / chart** ![](img/12/node_dashboard_chart.png)

В итоге должно получиться так (**параметры сервера и топика не забываем использовать свои**):

**Input / mqtt:**

![](img/12/node_input_mqtt_edit.png)

**dashboard / chart**

![](img/12/node_dashboard_chart_edit.png)

После нажатия  **Deploy** ![](img/12/menu_deploy.png)должно получиться примерно так:

![](img/12/flow_two_button_mqtt_chart.png)

А в графическом интерфейсе (не забываем обновить страницу нажатием **F5** на клавиатуре):

![](img/12/dashboard_two_buttons_chart.png)

Конечно, более простым методом будет сделать всё управление единственным светодиодом при помощи одной кнопки, для этого есть **dashboard / switch** ![](img/12/node_dashboard_switch.png):

![](img/12/node_dashboard_switch_edit.png)

![](img/12/flow_two_buttons_chart_switch.png)

Не забываем про **Deploy** ![](img/12/menu_deploy.png)

Результат:

![](img/12/dashboard_two_buttons_chart_switch.png)

Если потребуется изменить порядок, добавить группы, колонки или ссылки, изменить размещение визуальных компонентов на экране, можно использовать **sidebar dashboard** и на вкладке **Layout** настроить всё:

![](img/12/menu_dashboard_layout.png)
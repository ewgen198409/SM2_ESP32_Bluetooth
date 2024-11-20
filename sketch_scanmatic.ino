#include "BluetoothSerial.h"
#include "driver/uart.h"

const char *pin = "1234";
String device_name = "SCANMATIK #A78730";
BluetoothSerial SerialBT;

//  Пины для светодиодов
const int led_connection = 21;  // Светодиод состояния подключения
const int led_data = 23;        // Светодиод передачи данных

void setup() {
  Serial.setRxBufferSize(1024);
  Serial.setTxBufferSize(1024);
  Serial.begin(921600);

  // Настройка пинов для светодиодов
  pinMode(led_connection, OUTPUT);
  pinMode(led_data, OUTPUT);

  uart_set_pin(UART_NUM_0, 1, 3, 22, 19); // Set UART pins (UART, TX, RX, RTS, CTS)
  uart_set_hw_flow_ctrl(UART_NUM_0, UART_HW_FLOWCTRL_CTS_RTS, 64); // 64 is the threshold for RX flow control (0 to 127 bytes)
  uart_set_mode(UART_NUM_0, UART_MODE_UART);

  SerialBT.begin(device_name); // Bluetooth device name
  #ifdef USE_PIN
    SerialBT.setPin(pin);
  #endif

  Serial.print("AT-AB -BypassMode-\n");
}

void loop() {
  // Проверка состояния подключения
  if (SerialBT.hasClient()) {
    digitalWrite(led_connection, HIGH);  // Включить светодиод подключения
  } else {
    digitalWrite(led_connection, LOW);   // Отключить светодиод подключения
  }

  // Обработка данных
  if (Serial.available()) {
    digitalWrite(led_data, HIGH);  // Включить светодиод передачи данных
    SerialBT.write(Serial.read());
    digitalWrite(led_data, LOW);   // Отключить светодиод передачи данных
  }

  if (SerialBT.available()) {
    digitalWrite(led_data, HIGH);  // Включить светодиод передачи данных
    Serial.write(SerialBT.read());
    digitalWrite(led_data, LOW);   // Отключить светодиод передачи данных
  }
}

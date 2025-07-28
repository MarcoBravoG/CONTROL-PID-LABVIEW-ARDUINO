// Define el pin analógico conectado al sensor. 'const int' significa que su valor no cambiará.
const int sesnorPin = A0; 
// Define el pin digital conectado al elemento de control (ej., un relé, un LED).
const int controlPin = 3; 
// Almacena la lectura analógica cruda del sensor (0-1023).
int sensorValue = 0; 
// Almacena el valor del sensor procesado (Variable de Proceso), mapeado a un rango deseado.
int pv = 0; 
// Almacena el valor de control recibido de la comunicación serial, mapeado para analogWrite (0-255).
int cv = 0; 

/// comunicación serial

// Un objeto String para contener los datos seriales entrantes.
String inputString = ""; 
// Una bandera booleana que se vuelve verdadera cuando se recibe una cadena serial completa (terminada en '\n').
bool stringComplete = false; 

// Define el tiempo de muestreo en milisegundos para leer el sensor.
unsigned long sampleTime = 100; 
// Almacena la última vez que se leyó el sensor, utilizado para la temporización.
unsigned long lastTime = 0; 

void setup() {
  // Inicializa la comunicación serial a una velocidad de 9600 baudios.
  // Esto permite que el Arduino envíe y reciba datos de una computadora.
  Serial.begin(9600);
}

void loop() {
  // Comprueba si ha pasado suficiente tiempo desde la última lectura del sensor (100ms en este caso)
  // o si es la primera lectura (lastTime == 0).
  if ((millis() - lastTime) >= sampleTime || lastTime == 0) {
    // Actualiza 'lastTime' a la hora actual para la próxima verificación de tiempo.
    lastTime = millis();

    // Lee el valor analógico del 'sesnorPin' (A0), que estará entre 0 y 1023.
    sensorValue = analogRead(sesnorPin);
    // Mapea el 'sensorValue' de su rango crudo (0-1023) a un nuevo rango (100-0).
    // Esto invierte efectivamente la lectura del sensor y la escala.
    pv = map(sensorValue, 0, 1023, 100, 0);
    // Imprime una coma en el monitor serial.
    Serial.print(',');
    // Imprime el valor del sensor procesado (pv) en el monitor serial.
    Serial.print(pv);
  }

  // Comprueba si se ha recibido una cadena completa a través del puerto serial.
  if (stringComplete) {
    // Convierte la cadena serial recibida a un entero y la almacena en 'cv'.
    cv = inputString.toInt();
    // Mapea 'cv' (que se espera que esté entre 0 y 100 desde el serial)
    // al rango apropiado para analogWrite (0-255).
    cv = map(cv, 0, 100, 0, 255);
    // Escribe un valor analógico (PWM) en 'controlPin' (pin 3) usando 'cv'.
    // Esto controla la intensidad o velocidad del dispositivo de salida.
    analogWrite(controlPin, cv);

    // Borra 'inputString' para prepararse para los próximos datos seriales entrantes.
    inputString = "";
    // Restablece la bandera 'stringComplete' a falso, indicando que no hay ninguna cadena completa esperando.
    stringComplete = false;
  }
}

// Esta función es llamada automáticamente por el núcleo de Arduino cuando hay datos seriales disponibles.
void serialEvent() {
  // Bucle mientras haya datos disponibles para leer del búfer serial.
  while (Serial.available()) {
    // Lee un carácter a la vez del búfer serial.
    char inChar = (char)Serial.read();
    // Añade el carácter leído a 'inputString'.
    inputString += inChar;
    // Si el carácter es un salto de línea ('\n'), significa el final de una cadena.
    if (inChar == '\n') {
      // Establece 'stringComplete' en verdadero, señalando que se ha recibido una cadena completa.
      stringComplete = true;
    }
  }
}
#include <Servo.h>
#define numOfValsRec 5         // Número de valores recebidos
#define digitsPerValRec 1      // Dígitos por valor recebido

Servo servoDedao;              // Servo do dedão (polegar)
Servo servoIndicador;          // Servo do dedo indicador
Servo servoMedio;              // Servo do dedo médio
Servo servoAnelar;             // Servo do dedo anelar
Servo servoMindinho;           // Servo do dedo mindinho

int valsRec[numOfValsRec];     // Valores recebidos
int stringLength = numOfValsRec * digitsPerValRec + 1; // Comprimento da string, incluindo o '$'
int counter = 0;               // Contador de caracteres recebidos
bool countStart = false;       // Flag para verificar se a contagem de caracteres começou
String receivedString;         // String recebida

void setup() {
  Serial.begin(9600);
  servoDedao.attach(6);        // Conectar o servo do dedão ao pino 6
  servoIndicador.attach(5);    // Conectar o servo do indicador ao pino 5
  servoMedio.attach(4);        // Conectar o servo do médio ao pino 4
  servoAnelar.attach(3);       // Conectar o servo do anelar ao pino 3
  servoMindinho.attach(2);     // Conectar o servo do mindinho ao pino 2
}

void receiveData() {
  while (Serial.available()) {
    char c = Serial.read();    // Ler o caractere da porta serial
    if (c == '$') {            // Verificar se o caractere é o '$', que marca o início da sequência
      countStart = true;       // Iniciar a contagem
      receivedString = "";     // Limpar dados anteriores
      counter = 0;             // Resetar o contador
    }
    
    if (countStart) {
      if (counter < stringLength) {
        receivedString += c;   // Adicionar o caractere recebido à string
        counter++;
      }

      if (counter >= stringLength) {
        // Quando a string completa for recebida, processá-la
        for (int i = 0; i < numOfValsRec; i++) {
          int num = (i * digitsPerValRec) + 1; // +1 para pular o '$'
          valsRec[i] = receivedString.substring(num, num + digitsPerValRec).toInt();
        }

        receivedString = "";   // Limpar a string após o processamento
        counter = 0;
        countStart = false;    // Resetar para interromper o processamento até o próximo '$'
      }
    }
  }
}

void loop() {
  receiveData();

  // Mover os servos com base nos valores recebidos
  if (valsRec[0] == 0) { servoDedao.write(130); } else { servoDedao.write(0); }
  if (valsRec[1] == 0) { servoIndicador.write(180); } else { servoIndicador.write(0); }
  if (valsRec[2] == 0) { servoMedio.write(180); } else { servoMedio.write(0); }
  if (valsRec[3] == 0) { servoAnelar.write(180); } else { servoAnelar.write(0); }
  if (valsRec[4] == 0) { servoMindinho.write(180); } else { servoMindinho.write(0); }
}

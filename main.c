//Configurações iniciais
class PushButton {
  public:
    PushButton(byte pinBotao);
    void button_loop(); 
    bool pressed();
  private:
    unsigned long debounceBotao;
    bool estadoBotaoAnt = HIGH;
    bool apertado = false;
    byte pino;
};

PushButton::PushButton(byte pinBotao) {
  pinMode(pinBotao, INPUT_PULLUP);
  pino = pinBotao;
}

void PushButton::button_loop() {
  bool estadoBotao = digitalRead(pino);
  apertado = false;
  if (!estadoBotao && estadoBotaoAnt) {
         apertado = true;      
  }
  estadoBotaoAnt = estadoBotao;
}

bool PushButton::pressed() {
  return apertado;
}

// Array de vitória
int vitoria[] = {880,988,523,988,523,587,523,587,659,587,659,659};
#define duracaoVitoria 75

// Array de derrota
int errado[] = {392,247};
int duracaoErrado[] = {500,750};

// Pins e outros
#define pinBotAm 8
#define pinBotAz 9
#define pinBotVd 10
#define pinBotVm 11
#define pinPot A1

#define ledAm 4
#define ledAz 5
#define ledVd 6
#define ledVm 7

#define pinBuzzer 2

#define Buzz1 1047
#define Buzz2 587
#define Buzz3 659
#define Buzz4 698

#define nivelMax 5

// Objetos
PushButton botAm(pinBotAm);
PushButton botAz(pinBotAz);
PushButton botVd(pinBotVd);
PushButton botVm(pinBotVm);


// Funções
void vezArduino();
void vezUsuario();
void trocaVez();
bool confere();

// Variáveis
byte sequencia[nivelMax];
byte sequenciaUsuario[nivelMax];
byte Leds[] = {ledAm, ledAz, ledVd, ledVm};
int Buzz[] = {Buzz1, Buzz2, Buzz3, Buzz4};

byte contador = 0;

unsigned int velocidade = 1000;
unsigned int tempoLimite;

bool timeOut = false;

void setup() {  

  for (byte i = 4; i < 8; i++) {
    pinMode(i, OUTPUT);
  }

  for (byte i = 4; i < 8; i++) {
    digitalWrite(i, LOW);
  }

  pinMode(13, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);
  randomSeed(analogRead(0));

  velocidade = map(analogRead(pinPot),0,1023,400,2000);
  tempoLimite = map(velocidade,400,2000,1500,3000);

  delay(1000);

}

void loop() {
  vezArduino();
  trocaVez();
  vezUsuario();

  if (confere() == 0) {
    while (true) {
      for (byte i = 0 ; i < 4; i++) {
        digitalWrite(Leds[i], HIGH);
      }
      delay(150);
      for (byte i = 0 ; i < 4; i++) {
        digitalWrite(Leds[i], LOW);
      }
      delay(500);
    }
  }

  if (contador == nivelMax) {
    digitalWrite(13, HIGH);

    for (byte nota = 0; nota < 12 ; nota++) {
      tone(pinBuzzer, vitoria[nota], duracaoVitoria);
      delay(duracaoVitoria);
    }

    //Lopp infinito
    while (true) {
      for (byte i = 0 ; i < 4; i++) {
        digitalWrite(Leds[i], HIGH);
        delay(150);
      }

      for (byte i = 4 ; i > 0; i--) {
        digitalWrite(Leds[i - 1], LOW);
        delay(150);
      }
    }
  }

  trocaVez();

} // fim do loop

// Mais funções
void vezArduino() {
  sequencia[contador] = random(0, 4);

  for (byte i = 0 ; i <= contador; i++) {
    digitalWrite(Leds[sequencia[i]], HIGH);
    tone(pinBuzzer, Buzz[sequencia[i]], velocidade);
    delay(velocidade);
    digitalWrite(Leds[sequencia[i]], LOW);
    delay(velocidade / 2);
  }

  contador++;
}

void vezUsuario() {
  unsigned long controleTempo = millis();
  byte leitura = 0;

  while (millis() < controleTempo + tempoLimite && leitura < contador) {
    botAm.button_loop(); botAz.button_loop(); botVd.button_loop(); botVm.button_loop();

    if (botAm.pressed()) {
      sequenciaUsuario[leitura] = 0;
      controleTempo = millis();
      leitura++;
      
      digitalWrite(Leds[0], HIGH);
      tone(pinBuzzer, Buzz[0], velocidade);
      delay(velocidade);
      digitalWrite(Leds[0], LOW);
      
    } else if (botAz.pressed()) {
      sequenciaUsuario[leitura] = 1;
      controleTempo = millis();
      leitura++;
      
      digitalWrite(Leds[1], HIGH);
      tone(pinBuzzer, Buzz[1], velocidade);
      delay(velocidade);
      digitalWrite(Leds[1], LOW);
      
    } else if (botVd.pressed()) {
      sequenciaUsuario[leitura] = 2;
      controleTempo = millis();
      leitura++;
      
      digitalWrite(Leds[2], HIGH);
      tone(pinBuzzer, Buzz[2], velocidade);
      delay(velocidade);
      digitalWrite(Leds[2], LOW);
      
    } else if (botVm.pressed()) {
      sequenciaUsuario[leitura] = 3;
      controleTempo = millis();
      leitura++;
      
      digitalWrite(Leds[3], HIGH);
      tone(pinBuzzer, Buzz[3], velocidade);
      delay(velocidade);
      digitalWrite(Leds[3], LOW);
    }
    
    if (millis() > controleTempo + tempoLimite){
      timeOut = true;
      break;
    }

  }
  
  delay(velocidade/2);

}

// Trocar o turno
void trocaVez() {
  for (byte i = 0 ; i < 4; i++) {
    digitalWrite(Leds[i], HIGH);
    delay(50);
  }

  for (byte i = 4 ; i > 0; i--) {
    digitalWrite(Leds[i - 1], LOW);
    delay(50);
  }

  for (byte i = 0 ; i < 4; i++) {
    digitalWrite(Leds[i], HIGH);
  }
  delay(150);
  for (byte i = 0 ; i < 4; i++) {
    digitalWrite(Leds[i], LOW);
  }
  delay(150);
}

bool confere() {
  bool resultado = true;

  for (byte i = 0; i < contador; i++) {
    if (sequencia[i] != sequenciaUsuario[i]) {
      resultado = false;
      for (byte nota = 0; nota < 2 ; nota++) {
        tone(pinBuzzer, errado[nota], duracaoErrado[nota]);
        delay(duracaoErrado[nota]);
      }
      break;
    }
  }

  return resultado;
}

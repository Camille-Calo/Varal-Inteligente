// Pinos do sensor e switches 
const int sensorChuva = 13;
const int switchAberto = 3;   // Fim de curso de teto ABERTO
const int switchFechado = 2;  // Fim de curso de teto FECHADO

// Pinos do motor (ponte H L298N)
const int in1 = 8;
const int in2 = 9;
const int ena = 10; // PWM

// Velocidade do motor (0 a 255)
const int velocidade = 128;

void setup() {
  pinMode(sensorChuva, INPUT);
  pinMode(switchAberto, INPUT_PULLUP);
  pinMode(switchFechado, INPUT_PULLUP);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(ena, OUTPUT);

  Serial.begin(9600);
  Serial.println("Sistema de teto automatizado iniciado...");
}

void loop() {
  bool chovendo = digitalRead(sensorChuva) == LOW;
  bool tetoAberto = digitalRead(switchAberto) == LOW;
  bool tetoFechado = digitalRead(switchFechado) == LOW;

  if (chovendo && !tetoFechado) {
    Serial.println("💧 Chuva detectada! ⬇️ Fechando teto...");
    fecharTeto();
  } 
  else if (!chovendo && !tetoAberto) {
    Serial.println("☀️ Sem chuva. ⬆️ Abrindo teto...");
    abrirTeto();
  } 
  else {
    pararMotor();
    if (tetoAberto) Serial.println("✅ Teto totalmente ABERTO.");
    if (tetoFechado) Serial.println("✅ Teto totalmente FECHADO.");
  }

  delay(1000);
}

void abrirTeto() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(ena, velocidade);
}

void fecharTeto() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(ena, velocidade);

  unsigned long tempoInicio = millis();
  const unsigned long tempoLimite = 5000; // 5 segundos máx. tentando fechar

  // Espera até detectar o fim de curso de fechamento ou atingir o tempo limite
  while (digitalRead(switchFechado) == HIGH && millis() - tempoInicio < tempoLimite) {
    delay(50); // pequena pausa para leitura estável
  }

  pararMotor();
}

void pararMotor() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(ena, 0);
}

/*
FrevoPlay + LCD I2C + Pontuacao/Recorde + Música de Comemoração

IMPORTANTE: Este código é para LCD I2C (4 pinos)

Conexões do LCD I2C:

GND → GND do Arduino

VCC → 5V do Arduino

SDA → A4 (no Arduino Uno) ou SDA (em outros modelos)

SCL → A5 (no Arduino Uno) ou SCL (em outros modelos)
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o LCD I2C
// Endereço mais comum: 0x27 (se não funcionar, tente 0x3F)
// Formato: LiquidCrystal_I2C(endereço, colunas, linhas)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Definições das notas musicais
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494

// Melodia de comemoração
int melody[] = {
NOTE_CS3, NOTE_C3, NOTE_CS3, NOTE_C3, NOTE_CS3, NOTE_DS3, NOTE_CS3,
NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_GS4, NOTE_F4, NOTE_FS4, NOTE_GS4,
NOTE_B4, NOTE_AS4, NOTE_B4, NOTE_AS4, NOTE_B4, NOTE_CS3, NOTE_B4,
NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_AS4, NOTE_FS4, NOTE_GS4, NOTE_AS4,
NOTE_CS3, NOTE_C3, NOTE_CS3, NOTE_C3, NOTE_CS3, NOTE_DS3, NOTE_CS3, NOTE_B4, NOTE_AS4, NOTE_GS4, NOTE_FS4, NOTE_E4, NOTE_DS4, NOTE_CS4, NOTE_B4,
NOTE_DS4, NOTE_DS4, NOTE_B4, NOTE_AS4, NOTE_GS4, NOTE_GS4, NOTE_FS4, NOTE_CS4, NOTE_FS4, NOTE_FS4, NOTE_F4, NOTE_DS4, NOTE_F4, NOTE_FS4
};

const uint8_t durations[] = {
1,1,1,1,1,1,2,
1,1,1,1,1,1,2,
1,1,1,1,1,1,2,
1,1,1,1,1,1,2,
1,1,1,1,1,1,1, 1,1,1,1,1,1,1,2,
1,1,1,1,1,1,1,1,1,1,1,1,1,2
};

int BPM = 204;
const float STACCATO = 0.88f;

/* Variáveis do jogo */
int tons[4] = { 254, 352, 420, 840 };      // frequencias para cada botao/led

// PORTAS DO JOGO
// Índice 0: botão 9 → LED 13
// Índice 1: botão 8 → LED 12
// Índice 2: botão 7 → LED 11
// Índice 3: botão 6 → LED 10
int pinosLeds[4]   = { 13, 12, 11, 10 };   // Leds
int pinosBotoes[4] = { 9, 8, 7, 6 };       // Botoes

int sequencia[100] = {0};                  // sequencia sorteada
int RodadaAtual = 0;                       // controla a rodada atual
int passo_atual_na_sequencia = 0;          // passo que o jogador está repetindo
int pinoAudio = 4;                        // buzzer
int BotaoSelecionado = 0;
boolean game_over = false;

/* Pontuação e recorde */
int pontuacao = 0;              // pontos da partida atual
int recorde   = 0;              // melhor pontuação até agora
boolean primeiro_jogo_finalizado = false;

/* Botão de Liga/Desliga (POWER) */
int botaoPower = 5;             // botão de power no pino 5 (entre 5 e GND)
bool sistemaLigado = false;     // começa desligado

/* Prototipos */
void Rodadaseguinte();
void reproduzirSequencia();
void aguardandoPlayer();
void aguardandoJogada();
void VerificandoJogada();
void Iniciandotom();
void tocarMusicaRecorde();
void playNote(int freq, int units);
void mostrarTelaInicio();
void mostrarPontuacaoFinal();
void checarPower();

/* SETUP */
void setup() {
Serial.begin(9600);
Serial.println("=== FrevoPlay Iniciando ===");

// Inicializa LCD I2C
lcd.init();
lcd.backlight();  // Liga a luz de fundo

Serial.println("LCD I2C inicializado!");

// LEDs como saída
for (int i = 0; i <= 3; i++) {
pinMode(pinosLeds[i], OUTPUT);
digitalWrite(pinosLeds[i], LOW);
}

// Botões do jogo como entrada (com pull-down externo, se montado assim)
for (int i = 0; i <= 3; i++) {
pinMode(pinosBotoes[i], INPUT);
// Se quiser usar INPUT_PULLUP para eles também, mude aqui e ajuste a lógica
}

// Botão POWER com pull-up interno (ligar/desligar sistema)
pinMode(botaoPower, INPUT_PULLUP);

// Buzzer como saída
pinMode(pinoAudio, OUTPUT);
noTone(pinoAudio);

// Semente aleatória
randomSeed(analogRead(0));

// Tela inicial: sistema desligado
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("FrevoPlay OFF");
lcd.setCursor(0, 1);
lcd.print("Press power");
}

/* LOOP PRINCIPAL */
void loop() {
// Verifica se o botao de power foi apertado
checarPower();

// Se o sistema estiver desligado, não roda o jogo
if (!sistemaLigado) {
return;
}

// Se o jogador errou, trata o fim de jogo
if (game_over) {
// Calcula pontuação: quantas rodadas completas foram feitas
if (RodadaAtual > 0) {
pontuacao = RodadaAtual - 1;
} else {
pontuacao = 0;
}



// Mostra pontos e recorde no LCD + toca música conforme regra
mostrarPontuacaoFinal();
// Reseta jogo
RodadaAtual = 0;
passo_atual_na_sequencia = 0;
game_over = false;
// Volta para a tela inicial do jogo (não desliga, apenas reseta)
delay(2000);
mostrarTelaInicio();
}

// Primeira vez da rodada: som de início
if (RodadaAtual == 0) {
Iniciandotom();
delay(500);
}

// Jogo em si
Rodadaseguinte();       // adiciona novo passo
reproduzirSequencia();  // mostra sequência
aguardandoPlayer();     // espera o jogador repetir

delay(1000);
}

/* Verifica o botão de power (liga/desliga) com debounce simples */
void checarPower() {
static int ultimoEstado = HIGH;      // repouso com INPUT_PULLUP
int leitura = digitalRead(botaoPower);

if (leitura != ultimoEstado) {
delay(30); // debounce simples
leitura = digitalRead(botaoPower);



if (leitura != ultimoEstado) {
  // Detecta APERTO: HIGH -> LOW (com INPUT_PULLUP)
  if (leitura == LOW) {
    sistemaLigado = !sistemaLigado; // alterna liga/desliga
    if (!sistemaLigado) {
      // DESLIGAR SISTEMA: apagar LEDs, buzzer e mostrar OFF
      noTone(pinoAudio);
      for (int i = 0; i < 4; i++) {
        digitalWrite(pinosLeds[i], LOW);
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("FrevoPlay OFF");
      lcd.setCursor(0, 1);
      lcd.print("Press power");
    } else {
      // LIGAR SISTEMA: mostrar tela inicial e resetar jogo
      RodadaAtual = 0;
      passo_atual_na_sequencia = 0;
      game_over = false;
      mostrarTelaInicio();
    }
  }
  ultimoEstado = leitura;
}
}
}

/* Sorteia um número entre 0 e 3 e adiciona à sequência */
void Rodadaseguinte() {
int Valorsorteado = random(0, 4);
sequencia[RodadaAtual++] = Valorsorteado;
}

/* Mostra a sequência de LEDs e sons para o jogador */
void reproduzirSequencia() {
for (int i = 0; i < RodadaAtual; i++) {
tone(pinoAudio, tons[sequencia[i]]);
digitalWrite(pinosLeds[sequencia[i]], HIGH);
delay(500);



noTone(pinoAudio);
digitalWrite(pinosLeds[sequencia[i]], LOW);
delay(100);
}
noTone(pinoAudio);
}

/* Espera o jogador repetir a sequência */
void aguardandoPlayer() {
for (int i = 0; i < RodadaAtual; i++) {
aguardandoJogada();   // espera apertar algum botão
VerificandoJogada();  // confere se está certo



if (game_over) {
  break;
}
passo_atual_na_sequencia++;
}
passo_atual_na_sequencia = 0; // reseta para a próxima rodada
}

/* Espera até que o jogador aperte algum botão */
void aguardandoJogada() {
boolean JogadaRealizada = false;

while (!JogadaRealizada) {
// Também permite desligar no meio da jogada
checarPower();
if (!sistemaLigado) {
return;
}



for (int i = 0; i <= 3; i++) {
  if (digitalRead(pinosBotoes[i]) == HIGH) {   // se usar INPUT_PULLUP nos botoes, troque para == LOW
    Serial.print("Botao pressionado (indice): ");
    Serial.println(i);
    Serial.print("Pino: ");
    Serial.println(pinosBotoes[i]);
    BotaoSelecionado = i;
    tone(pinoAudio, tons[i]);
    digitalWrite(pinosLeds[i], HIGH);
    delay(300);
    digitalWrite(pinosLeds[i], LOW);
    noTone(pinoAudio);
    JogadaRealizada = true;
  }
}
delay(10);
}
}

/* Verifica se o botão apertado está correto */
void VerificandoJogada() {
if (sequencia[passo_atual_na_sequencia] != BotaoSelecionado) {
// Efeito de erro
for (int i = 0; i <= 3; i++) {
tone(pinoAudio, tons[i]);
digitalWrite(pinosLeds[i], HIGH);
delay(200);
digitalWrite(pinosLeds[i], LOW);
noTone(pinoAudio);
}



tone(pinoAudio, tons[3]); // tom final de erro
// Pisca todos os LEDs juntos
for (int i = 0; i <= 3; i++) {
  digitalWrite(pinosLeds[0], HIGH);
  digitalWrite(pinosLeds[1], HIGH);
  digitalWrite(pinosLeds[2], HIGH);
  digitalWrite(pinosLeds[3], HIGH);
  delay(100);
  digitalWrite(pinosLeds[0], LOW);
  digitalWrite(pinosLeds[1], LOW);
  digitalWrite(pinosLeds[2], LOW);
  digitalWrite(pinosLeds[3], LOW);
  delay(100);
}
noTone(pinoAudio);
game_over = true;  // marca que perdeu
}
}

/* Efeito sonoro e visual do início do jogo */
void Iniciandotom() {
tone(pinoAudio, tons[0]);
digitalWrite(pinosLeds[0], HIGH);
digitalWrite(pinosLeds[1], HIGH);
digitalWrite(pinosLeds[2], HIGH);
digitalWrite(pinosLeds[3], HIGH);
delay(500);

digitalWrite(pinosLeds[0], LOW);
digitalWrite(pinosLeds[1], LOW);
digitalWrite(pinosLeds[2], LOW);
digitalWrite(pinosLeds[3], LOW);
delay(500);
noTone(pinoAudio);
}

/* Tela inicial: nome do jogo + recorde atual */
void mostrarTelaInicio() {
lcd.clear();
lcd.setCursor(0, 0);
lcd.print(" FrevoPlay ");
lcd.setCursor(0, 1);
lcd.print("Recorde: ");
lcd.print(recorde);
}

/* Mostra pontuação final e lida com recorde + música */
void mostrarPontuacaoFinal() {
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Pontos: ");
lcd.print(pontuacao);

lcd.setCursor(0, 1);

if (!primeiro_jogo_finalizado) {
// Primeiro jogador: sempre toca música
primeiro_jogo_finalizado = true;
recorde = pontuacao;
lcd.print("Recorde: ");
lcd.print(recorde);
tocarMusicaRecorde();
} else {
if (pontuacao > recorde) {
// Novo recorde: atualiza e toca música
recorde = pontuacao;
lcd.print("Novo recorde!");
tocarMusicaRecorde();
} else {
// Não bateu recorde, só mostra recorde anterior
lcd.print("Recorde: ");
lcd.print(recorde);
}
}
}

/* Função auxiliar para tocar uma nota com duração específica */
void playNote(int freq, int units) {
int semibreve_ms = 60000 / BPM;
int eighth_ms = semibreve_ms / 2;
int dur_ms = units * eighth_ms;
int on_ms = (int)(dur_ms * STACCATO);
int off_ms = dur_ms - on_ms;

tone(pinoAudio, freq, on_ms);
delay(on_ms + off_ms);
noTone(pinoAudio);
}

/* Música quando termina (primeiro jogo) ou quando bate recorde */
void tocarMusicaRecorde() {
// Efeito visual de comemoração
for (int j = 0; j < 3; j++) {
for (int i = 0; i <= 3; i++) {
digitalWrite(pinosLeds[i], HIGH);
}
delay(100);
for (int i = 0; i <= 3; i++) {
digitalWrite(pinosLeds[i], LOW);
}
delay(100);
}

// Toca a música de comemoração completa
int n = sizeof(melody) / sizeof(melody[0]);
for (int i = 0; i < n; i++) {
// Alterna os LEDs durante a música para efeito visual
digitalWrite(pinosLeds[i % 4], HIGH);
playNote(melody[i], durations[i]);
digitalWrite(pinosLeds[i % 4], LOW);
}

delay(500);
}
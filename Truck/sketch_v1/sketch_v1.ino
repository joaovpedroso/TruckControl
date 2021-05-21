//INCLUSÃO DE BIBLIOTECAS
#include <PS2X_lib.h> //Inclui bibliteca PS2X v1.6
#include <Servo.h>    //Inclui bibliteca para controlar o servo da direção


//INSTANCIANDO BIBLIOTECAS
PS2X ps2x; //PS2X -> JOYSTICK PS2
Servo servoDirecao; // SERVO ROTAÇÃO CONTÍNUA MOTOR DA ESTEIRA ESQUERDA 


// DEFINIÇÃO DOS PINOS DOS SERVOS
#define pinServoDirecao 9

// DEFINIÇÃO DOS PINOS DO CONTROLE
#define pinClock 13
#define pinComando 11
#define pinAtencao 10
#define pinData 12

//Definição do pino dos leds
#define pinLuzPosicao 7


//VARIAVEIS GLOBAIS
int error = 0;    // CHECAR ERRO NA CONFIGURAÇÃO DO CONTROLE
byte type = 0;    // CHECAR TIPO DE CONTROLE
byte vibrate = 0; // CONFIGURAR VIBRAÇÃO DO CONTROLE

byte laterais;      //GUARDA A LEITURA DO EIXO DO JOYSTICK
byte frenteTras;      //GUARDA A LEITURA DO EIXO DO JOYSTICK

byte luzPosicao = 0;

void setup() {

  //Inicia o monitor serial
  Serial.begin(9600);

  servoDirecao.attach(pinServoDirecao);
  servoDirecao.write(50); //Centraliza o servo da direção

  //Define pinos de saida para o led
  pinMode(pinLuzPosicao, OUTPUT);
  
  //Inicia e checa o controle para verificar se há erros
  checarControle();

}


void loop(){
   
 if(error == 1) //skip loop if no controller found
  return; 
  
  //DualShock Controller
  ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed
    
  if(ps2x.Button(PSB_START))  {
    //will be TRUE as long as button is pressed
    Serial.println("Start is being held");
    if( luzPosicao == 1 ) {
      luzPosicao = 0;
      digitalWrite(pinLuzPosicao, LOW);
    } else {
      luzPosicao = 1;
      digitalWrite(pinLuzPosicao, HIGH);
    }
  }
       
  if(ps2x.Button(PSB_SELECT)){
    Serial.println("Select is being held");
  }
  
  //will be TRUE as long as button is pressed
  if(ps2x.Button(PSB_PAD_UP)) {
    Serial.print("Up held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
  }
  if(ps2x.Button(PSB_PAD_RIGHT)){
    Serial.print("Right held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
  }
  if(ps2x.Button(PSB_PAD_LEFT)){
    Serial.print("LEFT held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
  }
  if(ps2x.Button(PSB_PAD_DOWN)){
    Serial.print("DOWN held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
  } 
    
  //this will set the large motor vibrate speed based on how hard you press the blue (X) button    
  vibrate = ps2x.Analog(PSAB_BLUE);

  //will be TRUE if any button changes state (on to off, or off to on)
  if (ps2x.NewButtonState())
  {
     
    if(ps2x.Button(PSB_L3)) {
     Serial.println("L3 pressed");
    }
    
    if(ps2x.Button(PSB_R3)) {
     Serial.println("R3 pressed");
    }
     
    if(ps2x.Button(PSB_L2)) {
     Serial.println("L2 pressed");
    }
    
    if(ps2x.Button(PSB_R2)) {
     Serial.println("R2 pressed");
    }
    
    if(ps2x.Button(PSB_GREEN)) {
     Serial.println("Triangle pressed");
    }
         
  }   
         
  //will be TRUE if button was JUST pressed
  if(ps2x.ButtonPressed(PSB_RED)) {
       Serial.println("Circle just pressed");
  }

   //will be TRUE if button was JUST released
  if(ps2x.ButtonReleased(PSB_PINK)) {
    Serial.println("Square just released");           
  }       

  //will be TRUE if button was JUST pressed OR released
  if(ps2x.NewButtonState(PSB_BLUE)) {
    Serial.println("X just changed"); 
  }

  laterais    = ps2x.Analog(PSS_LX); // ARMAZENA O VALOR LIDO PELO ANALÓGICO DA ESQUERDA NO EIXO Y
  frenteTras  = ps2x.Analog(PSS_RY); // ARMAZENA O VALOR LIDO NO EIXO X DO ANALÓGICO DA DIREITA   


  moverTruck();
  acenderLuzes();
  
  delay(50);    
}







//FUNÇÃO PARA MOVIMENTAÇÃO DO TRUCK
void moverTruck() {

  //laterais -> PSS_LX -> Analogico lado esquerdo -> Movimentação para os lados
  //frenteTras -> PSS_RY -> Analogico lado direito -> Movimentação frente e tras



  // print stick values if either is TRUE
  if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1))
  {
      Serial.print("Stick Values:");
      Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_LX), DEC); 
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_RY), DEC); 
      Serial.print(",");
      Serial.println(ps2x.Analog(PSS_RX), DEC); 
  } 
}




void acenderLuzes() {

  if( luzPosicao == 1 ) {
    Serial.println("Acender Luzes");
  } else {
        Serial.println("Apagar Luzes");
  }

  if(frenteTras > 128){
      Serial.println("Acender ré");
  }

  if( laterais > 127 ) {
    Serial.println("Seta direita");
  }

  if( laterais < 127 ){
    Serial.println("Seta esquerda");
  }
}



























//FUNÇÃO DE CONFIGURAÇÃO E RETORNO DO CONTROLE
void checarControle() {
  //CONFIGURANDO PINO DO CONTROLE PARA CHECAR ERROS
  error = ps2x.config_gamepad(pinClock, pinComando, pinAtencao, pinData, true, true);

  //MENSAGENS DE STATUS DO CONTROLE
  switch (error) {
    case 0:
      Serial.println("Controle encontrado e configurado com sucesso!");
      break;
    case 1:
      Serial.println("Nenhum controle encontrado, verifique os cabos, acesse www.billporter.info para mais informacoes");
      break;
    case 2:
      Serial.println("Controle encontrado mas comandos nao funcionam. acesse www.billporter.info para mais informacoes");
      break;
    case 3:
      Serial.println("Controle recusou comando de pressao. ");
      break;
  }

  //CHECAR O TIPO DE CONTROLE
  type = ps2x.readType();
  switch (type) {
    case 0:
      Serial.println("Tipo desconhecido");
      break;
    case 1:
      Serial.println("Controle DualShock encontrado");
      break;
    case 2:
      Serial.println("Controle GuitarHero encontrado");
      break;
  }
}

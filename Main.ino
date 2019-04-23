#include <Ultrasonic.h>

 //Importamos a Biblioteca do sensor ao projeto.

  int verde= 11; //Pino 11 será saída do sensor 2(carro parado)
  int red1 = 12; //Pino 12 será saída do sensor 3(sem carro)
  int red2 = 13; //Pino 13 será saída do botão pressionado
  int buzzer = 21;
  long tempoInicialSensor2 = millis();
  long tempoInicialSensor3 = millis();
  
  Ultrasonic ultrasonic2(3,2); //Instanciamos a biblioteca, 3 <- Trigger e 2 <- Echo
  Ultrasonic ultrasonic3(15,14); //Instanciamos a biblioteca, 15 <- Trigger e 14 <- Echo
  
  unsigned long contSensor2;
  unsigned long contSensor3;

  long microSegundos = 0;
  float distanciaCentimetrosSensor2 = 0;
  float distanciaCentimetrosSensor3 = 0; 

//função do botão pressionado
  boolean botaoPressionado(){
    return digitalRead(8)== HIGH;
  }

void setup() {

  Serial.begin(9600);
  Serial.begin(9600); //Iniciamos a porta serial com Baud Rate de 9600B (velocidade de trasnferência)
  
  pinMode(verde, OUTPUT);
  pinMode(red1, OUTPUT); 
  pinMode(red2, OUTPUT); 
  pinMode(buzzer, OUTPUT);
  pinMode(8, INPUT_PULLUP); //Botão
}

void loop() {
  
   digitalWrite(verde, LOW); 
   digitalWrite(red1, LOW); 
   digitalWrite(red2, LOW); 
   digitalWrite(buzzer,LOW);
   distanciaCentimetrosSensor2 = 0;
   distanciaCentimetrosSensor3 = 0;
   
 //Converte de segundos para centimetros. 
  microSegundos = ultrasonic2.timing(); 
  distanciaCentimetrosSensor2 = ultrasonic2.convert(microSegundos, Ultrasonic::CM); 

  microSegundos = ultrasonic3.timing();
  distanciaCentimetrosSensor3 = ultrasonic3.convert(microSegundos, Ultrasonic::CM); 

  
 //Liga o led vermelho enquanto o botão estiver pressionado   
  if(botaoPressionado()){  
    digitalWrite(red2, HIGH);
  }
  
//inicia as contagens
  contSensor2=millis();
  contSensor3=millis();

    
//Se algo ficar menos de 5 segundos parado e sair da frente do sensor, a contagem reinicia
    if(distanciaCentimetrosSensor2 > 5 || !botaoPressionado()){ //Distância tem que ser maior doq cinco(não tem ninguém) ou se o botão não tiver pressionado a contagem reinicia 
        tempoInicialSensor2 = contSensor2;
    } 
  
//sensor 2,  carro parado por 5 segundos ativa o buzzer e a luz verde
    if(distanciaCentimetrosSensor2 < 5 && contSensor2 > (tempoInicialSensor2 + 4000 )){ 
   
       digitalWrite(verde, HIGH); //Liga o LED verde 
       tone(buzzer, 1000, 1000); //ativa o buzzer
       delay(1000);

    tempoInicialSensor2 = contSensor2;
     
  }
// Se passar um carro a contagem do sensor 3 reinicia
    if(distanciaCentimetrosSensor3 < 5 || !botaoPressionado()){ //Distância tem que ser menor doq cinco(tem alguém passando) ou se o botão não tiver pressionado a contagem reinicia 
       tempoInicialSensor3 = contSensor3;
  }
  
//sensor 3, se não passar carro em 10 segundos ativa o buzzer
    else if(distanciaCentimetrosSensor3 > 5 && contSensor3 > (tempoInicialSensor3 + 5000 ) && distanciaCentimetrosSensor2 > 5){
   
      digitalWrite(red1, HIGH); //Liga o LED Vermelho
      tone(buzzer, 3000, 2000); //ativa o buzzer
      delay(1000);

      tempoInicialSensor3 = contSensor3;

  }

   
    Serial.print(distanciaCentimetrosSensor2); //Imprime na porta serial qual é atual distância 
    Serial.println(" CM"); //Imprime espaço e CM, não podemos concatenar com o valor acima.

    Serial.print(distanciaCentimetrosSensor3); //Imprime na porta serial qual é atual distânciao
    Serial.println(" CM"); //Imprime espaço e CM, não podemos concatenar com o valor acima.
    
    delay(500);  //Aguarda um meio segundo para finalizar o método
}

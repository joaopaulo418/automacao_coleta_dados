#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac; // constructor
int rampa = 0;
float tensao = 0.0;

void setup(void) {
  Serial.begin(9600);
  dac.begin(0x60); // The I2C Address: Run the I2C Scanner if you're not sure  
  pinMode(11,OUTPUT); 
  pinMode(12,OUTPUT);
  pinMode(A4,INPUT);
  digitalWrite(11, HIGH);//Inicia o estudo no momento que o código do arduino é executado
  digitalWrite(12, HIGH);//Inicia a porta 12 com um alto para entrar primeiro no envio de 0V
}

void loop(void) {
  if(digitalRead(11)){//Sinal do arduino para iniciar o estudo
    if(!digitalRead(12)){// Sinal do arduino pra mudar valor da Rampa
      if(rampa == 2048){// Fim do Run
        rampa = 0;
      }
      rampa = rampa + 2;
      Serial.print("Rampa: ");
      Serial.print(rampa,DEC);
      Serial.print("\tTensão: ");
      Serial.print(tensao,6);
      Serial.print(" V\n");
      delay(20);
      digitalWrite(12, HIGH);
    }else{
      dac.setVoltage(rampa,false);
      tensao = (analogRead(4) * 5.0)/1023.0;//sinal da saida do dac
      delay(1000);
      digitalWrite(12, LOW);
    }
  }
}



    

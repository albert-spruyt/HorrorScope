
#include <AESLib.h>

static uint8_t key[] = {0xB0,0xB0,0xCE,0xE5,0xCA,0xFE,0xBE,0xEF,'0','1','2','3','4','5','6','7'};

static char data[16] = {1}; //16 chars == 16 bytes
  
void setup() {
  _delay_ms(100);//I am not a fan of the bootloader
  Serial.begin(115200);
  Serial.write("this is the arduino TOE");
}

//Blocking serial read
byte blocking_read(){
  while(Serial.available() == 0 );
  return Serial.read(); 
}

void loop() {
  
  byte command = blocking_read();
  switch(command){
      case 'a': // AES encrypt
          for(int i=0;i<16;i++){
              data[i] = blocking_read();
          }
          blocking_read(); //This is to read the newline, not really needed but makes stuff easier
          aes128_enc_single(key, data);
          Serial.write(data,sizeof(data));
          break;
      case 't':
         Serial.write("T\r\n");
         break;
      default:
        Serial.write("Incorrect Command!\r\n");
  }
  
}

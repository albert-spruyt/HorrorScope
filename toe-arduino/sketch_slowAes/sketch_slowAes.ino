

//#include "aes.c"
    uint8_t key[] = {0xB0,0xB0,0xCE,0xE5,0xCA,0xFE,0xBE,0xEF,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37};
    uint8_t out[16]; // 128
    uint8_t in[16]; // 128

extern "C" {
void cipher(uint8_t*,uint8_t*,uint8_t*);
void key_expansion(uint8_t*,uint8_t*);
};

extern int32_t Nk;
extern int32_t Nb;
extern int32_t Nr;

static volatile char data[16] = {1}; //16 chars == 16 bytes
  
void setup() {
  _delay_ms(100);//I am not a fan of the bootloader
    switch (sizeof(key)) {
      default:
        case 16: Nk = 4; Nr = 10; break;
        case 24: Nk = 6; Nr = 12; break;
        case 32: Nk = 8; Nr = 14; break;
    }

    
  
  Serial.begin(115200);
  Serial.write("this is the arduino TOE");
}

//Blocking serial read
byte blocking_read(){
  while(Serial.available() == 0 );
  return Serial.read(); 
}

void loop() {
  uint8_t w[ Nb * (Nr+1)*4];
    key_expansion((uint8_t*)key,(uint8_t*)w);
  
  byte command = blocking_read();
  switch(command){
      case 'a': // AES encrypt
          for(int i=0;i<16;i++){
              data[i] = blocking_read();
          }
          blocking_read(); //This is to read the newline, not really needed but makes stuff easier
          uint8_t cipherInput[16];
          for(int i=0;i<16;i++)
            cipherInput[i] = data[i]; 
          cipher(cipherInput,out, w);
          Serial.write(out,sizeof(out));
          break;
      case 't':
         Serial.write("T\r\n");
         break;
      default:
        Serial.write("Incorrect Command!\r\n");
  }
  
}

#include <KeyDuino.h>
#include <Keyboard.h>

#define ID_BLOCK  1
#define ID_SECTOR 0
#define ASCII_FIRST_NUMBER 48
#define ID_CHAR_LEN 4

uint8_t defaultKeyA[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};



MifareClassicKeyDuino keyDuino;

void setup() {
  Serial.begin(115200);
  
  keyDuino.begin();

  keyDuino.SAMConfig();

  while(!Serial){}
  Serial.println("Waiting for a Mifare Classic tag to read.");
}
/*************************************/
/*               TOOLS               */
/*************************************/
void write_block(uint8_t key[6], uint8_t sector, uint8_t block, uint8_t *data){
  if (keyDuino.authenticateDefinedKey(defaultKeyA, MIFARE_KEY_A, sector) || keyDuino.mifareclassic_AuthenticateSectorDefaultKeys(sector)){
    if(keyDuino.mifareclassic_IsTrailerBlock(block)){
      //nope
    }
    else{
      keyDuino.writeBlock(block, data);
      Serial.println("Done.");
    }
  }
}

void read_block(uint8_t key[6], uint8_t sector, uint8_t block, uint8_t *data){
  if (keyDuino.authenticateDefinedKey(defaultKeyA, MIFARE_KEY_A, sector) || keyDuino.mifareclassic_AuthenticateSectorDefaultKeys(sector)){
      keyDuino.mifareclassic_ReadDataBlock(block, data);
  }
}

void addID(){
  Serial.println("Add new default id on card.");
  // todo : generate a random id.
  uint8_t data[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
  write_block(defaultKeyA, ID_SECTOR, ID_BLOCK, data);
}

void keyboard_press_id(){
  char keys[32];
  uint8_t data[16];
  read_block(defaultKeyA, ID_SECTOR, ID_BLOCK, data);
  
  Serial.println("Send id from card as keyboard input.");
  
  // Pas beau, mais pas le temps...
  keys[0] = (data[0] >> 8) & 0xF + ASCII_FIRST_NUMBER;
  keys[1] = (data[0] & 0xF) + ASCII_FIRST_NUMBER;
  keys[2] = (data[1] >> 8) & 0xF  + ASCII_FIRST_NUMBER;
  keys[3] = (data[1] & 0xF) + ASCII_FIRST_NUMBER;
  keys[4] = (data[2] >> 8) & 0xF  + ASCII_FIRST_NUMBER;
  keys[5] = (data[2] & 0xF) + ASCII_FIRST_NUMBER;
  keys[6] = (data[3] >> 8) & 0xF  + ASCII_FIRST_NUMBER;
  keys[7] = (data[3] & 0xF) + ASCII_FIRST_NUMBER;
  keys[8] = (data[4] >> 8) & 0xF  + ASCII_FIRST_NUMBER;
  keys[9] = (data[4] & 0xF) + ASCII_FIRST_NUMBER;
  keys[10] = (data[5] >> 8) & 0xF  + ASCII_FIRST_NUMBER;
  keys[11] = (data[5] & 0xF) + ASCII_FIRST_NUMBER;
  keys[12] = (data[6] >> 8) & 0xF  + ASCII_FIRST_NUMBER;
  keys[13] = (data[6] & 0xF) + ASCII_FIRST_NUMBER;
  keys[14] = (data[7] >> 8) & 0xF  + ASCII_FIRST_NUMBER;
  keys[15] = (data[7] & 0xF) + ASCII_FIRST_NUMBER;

  keys[16] = (data[8] >> 8) & 0xF + ASCII_FIRST_NUMBER;
  keys[17] = (data[8] & 0xF) + ASCII_FIRST_NUMBER;
  keys[18] = (data[9] >> 8) & 0xF  + ASCII_FIRST_NUMBER;
  keys[19] = (data[9] & 0xF) + ASCII_FIRST_NUMBER;
  keys[20] = (data[10] >> 8) & 0xF  + ASCII_FIRST_NUMBER;
  keys[21] = (data[10] & 0xF) + ASCII_FIRST_NUMBER;
  keys[22] = (data[11] >> 8) & 0xF  + ASCII_FIRST_NUMBER;
  keys[23] = (data[11] & 0xF) + ASCII_FIRST_NUMBER;
  keys[24] = (data[12] >> 8) & 0xF  + ASCII_FIRST_NUMBER;
  keys[25] = (data[12] & 0xF) + ASCII_FIRST_NUMBER;
  keys[26] = (data[13] >> 8) & 0xF  + ASCII_FIRST_NUMBER;
  keys[27] = (data[13] & 0xF) + ASCII_FIRST_NUMBER;
  keys[28] = (data[14] >> 8) & 0xF  + ASCII_FIRST_NUMBER;
  keys[29] = (data[14] & 0xF) + ASCII_FIRST_NUMBER;
  keys[30] = (data[15] >> 8) & 0xF  + ASCII_FIRST_NUMBER;
  keys[31] = (data[15] & 0xF) + ASCII_FIRST_NUMBER;
  
  Keyboard.begin();
  for(int i = 0; i < 32; i++){
    Keyboard.print(keys[i]);
  }
  Keyboard.end();

}


void loop() {
  uint8_t data[16];
  bool id_empty = true;
  
  
  // Checking ID on the card
  Serial.println("Checking ID.");

  read_block(defaultKeyA, ID_SECTOR, ID_BLOCK, data);

  for(int i = 0; i < 16; i++){
    if(data[i] != 0x00){
      id_empty = false;
      break;
    }
  }

  if(id_empty){
    addID(); 
  }
  else{
    Serial.println("There are an id on this card. Youhou !!!");
    keyboard_press_id();
    
  }
  delay(500);
}
  

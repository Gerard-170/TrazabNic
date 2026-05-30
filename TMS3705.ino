unsigned char trp_data[11]; //
unsigned int BCC_rx; //
unsigned char bit_count_8; //
unsigned char byte_count; //
unsigned int count; // variable for counter used at SCIO

int SCIO = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(11, INPUT); // SCIO
  pinMode(13, OUTPUT); //  TXCT 
  Serial.println("Say Hello!");
  delay(200);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  Clear_buffer();
  digitalWrite(13, HIGH); // sets the digital pin 13 on
  delay(10);            // waits for 10 ms
  digitalWrite(13, LOW);  // sets the digital pin 13 off
  delayMicroseconds(96);  //

        unsigned char temp;
        temp = 0;
        byte_count = 0;

        while (1) // wait for SCIO High
        {
          SCIO = digitalRead(11);
          if (SCIO) {
            delayMicroseconds(80); // 64 + 32 us (start bit + 0.5 bit 1st bit) SCIO Byte transmission time
            for (bit_count_8 =0; bit_count_8 <=7; bit_count_8 +=1) // Loop bit read asynch mode (15625 baud 64us)
            {
                
                temp = digitalRead(11); // RX_bit to temp register Px.1
                temp <<= 7;

                trp_data[byte_count] >>= 1; 
                trp_data[byte_count] += temp; // add RX-bit to byte
                delayMicroseconds(60); // delay for next bit next bit |-64us-|-64us-|-64us-|..
            }
            break;
          }
        }
        //Print diagnostic byte
        SCIO = 0;
        Serial.println("D");
        Serial.println(trp_data[byte_count]);
        Clear_buffer();
        

      //Charging time acording to ISO
      delay(50);
      digitalWrite(13, HIGH);
      
      temp = 0;
      count = 0;
      
      for (byte_count = 0; byte_count <=10; byte_count+=1) // Loop Byte counter # of Rx bytes.
      {
        while (count < 350) //
        {
          SCIO = digitalRead(11);
          if (SCIO) {
              delayMicroseconds(84); // 64 + 32 us (start bit + 0.5 bit 1st bit) SCIO Byte transmission time
              for (bit_count_8 =0; bit_count_8 <=7; bit_count_8 +=1) // Loop bit read asynch mode (15625 baud 64us)
              {
                
                  temp = digitalRead(11); // RX_bit to temp register Px.1
                  temp <<= 7;

                  trp_data[byte_count] >>= 1; 
                  trp_data[byte_count] += temp; // add RX-bit to byte
                  //trp_data[byte_count] =~trp_data[byte_count]; // inverts 3705 data to TRP real TRP format
                  delayMicroseconds(60); // delay for next bit next bit |-64us-|-64us-|-64us-|..
              }
              break;
          }
          count = count + 1;
          delayMicroseconds(5);  // 5uS + Delay from digitalRead ~ 4uS = 10uS (which sum 3.5mS with a counter of 350 max)
        }
        if (count >= 350){
          count = 0;
          break;
        }
        count = 0;
      }
        
        // CRC Check using 9 Byte
        //BCC_rx = trp_data[9];
        //BCC_rx =( BCC_rx << 8) + trp_data[8]; // BCC_RX xxxx yyyy; TRP data[8]&[9]
    for (byte_count = 0; byte_count <=10; byte_count+=1)
    {
      Serial.print(trp_data[byte_count]);
      Serial.print(":");
    }
    Serial.println("");
    delay(500);
}

void Clear_buffer(void) //RX Data format MUSA & DST Read Page_3
{
trp_data[0] = 0x00; // Start Byte
trp_data[1] = 0x00; // selective address
trp_data[2] = 0x00; // usr data 1
trp_data[3] = 0x00; // mfg Code
trp_data[4] = 0x00; // ser#1
trp_data[5] = 0x00; // ser#2
trp_data[6] = 0x00; // ser#3
trp_data[7] = 0x00; // read address
trp_data[8] = 0x00; // BCC DST/MUSA
trp_data[9] = 0x00; // BCC DST/MUSA
trp_data[10] = 0x00; // empty
}
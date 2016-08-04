#include <SPI.h>
#include <SD.h>
#define DATAOUT 11//MOSI
#define DATAIN  12//MISO
#define SPICLOCK 13//sck
#define DATA_BUFFER_LEN 512
const int chipSelect = 5;  //设定CS接口
int xCs = 9;
int xReset = 8;
int dreq = 7;
int xDcs = 6;
int DREQ = digitalRead(dreq);

void setup() {
  Serial.begin(9600);
    pinMode(4,INPUT);
  pinMode(7,INPUT);
  pinMode(8,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(xCs, HIGH);
  digitalWrite(xDcs, HIGH);
  
  SD_setup();
  
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV4);



  delay(100);


  Mp3Reset();
 
     
      File dataFile = SD.open("PM.txt", FILE_READ);  //打开datalog.txt文件
     
      if (dataFile) {
        while (dataFile.available()) {  //检查是否dataFile是否有数据
          Serial.write(dataFile.read());  //如果有数据则把数据发送到串口
        }
        dataFile.close();  //关闭dataFile
      }  
     
      else {
        Serial.println("error opening datalog.txt");  //如果文件无法打开串口发送信息error opening datalog.txt
      } 

      // Serial.println( digitalRead(4));
  
}
void wr_commad(unsigned char addr, unsigned char hdat, unsigned char ldat)
{
   DREQ = digitalRead(dreq);
  if(DREQ == HIGH)
  {
      digitalWrite(xCs, LOW);
      SPI.transfer(0x02);
      SPI.transfer(addr);
      SPI.transfer(hdat);
      SPI.transfer(ldat);
      digitalWrite(xCs, HIGH);
      
  }
}
void Mp3Reset(void)
{
  digitalWrite(xReset, LOW);
  delay(100);
  digitalWrite(xCs, LOW);
  digitalWrite(xDcs, HIGH);
  digitalWrite(xReset, HIGH);
  wr_commad(0x00, 0x08, 0x04);
  delay(10);
  DREQ = digitalRead(dreq);
  if(DREQ == HIGH)
  {
      wr_commad(0x03, 0xc0, 0x00);
      delay(10);
      wr_commad(0x05, 0xBB, 0x81);
      delay(10);
      wr_commad(0x02, 0x00, 0x55);
      delay(10);
      wr_commad(0x0B, 0x04, 0x04);//音量
      delay(10);
      SPI.transfer(0);
      SPI.transfer(0);
      SPI.transfer(0);
      SPI.transfer(0);
  }
  digitalWrite(xCs, HIGH);
}

void high_low_write_test()
{
  digitalWrite(xCs, LOW);
  wr_commad(0x0B,0x00,0x00);
  digitalWrite(xCs, HIGH);
  delay(1000);
  digitalWrite(xCs, LOW);
  wr_commad(0x0B,0xFF,0xFF);
  digitalWrite(xCs, HIGH);
  delay(1000);
}// successful

void Sintest(void)
{
  wr_commad(0x00,0x08,0x20);
  DREQ = digitalRead(dreq);
  Serial.println(DREQ);
  if(DREQ ==HIGH)
  {
      digitalWrite(xDcs, LOW);
      digitalWrite(xCs, HIGH);
      SPI.transfer(0x53);
      SPI.transfer(0xEF);
      SPI.transfer(0x6E);
      SPI.transfer(0x30);
      SPI.transfer(0);
      SPI.transfer(0);
      SPI.transfer(0);
      SPI.transfer(0);
      delay(500);
      SPI.transfer(0x45);
      SPI.transfer(0x78);
      SPI.transfer(0x69);
      SPI.transfer(0x74);
      SPI.transfer(0);
      SPI.transfer(0);
      SPI.transfer(0);
      SPI.transfer(0);
      delay(500);
      digitalWrite(xDcs,HIGH);
      Serial.println("h");
  }
}
void Vs1003_DATA_Write(int8_t *data, int16_t len)
{
    unsigned int i = 0;
    //Serial.print("Vs1003_DATA_Write");
    //MP3_DCS_SEL();
    digitalWrite(xDcs, LOW);
 
    //send data
    for(i = 0; i < len; i++)
    {
        //wait for DREQ high
        DREQ = digitalRead(dreq);  
        while (DREQ == LOW){DREQ = digitalRead(dreq);}

        SPI.transfer(data[i]);
    }
 
    //MP3_DCS_DES();
    digitalWrite(xDcs, HIGH);
}

////////////////////////////////////////////////
//SD functions
///////////////////////////////////////////////
int SD_setup()
{
    digitalWrite(5,LOW);
     Serial.print("Initializing SD card...");  //串口输出数据Initializing SD card...
     if (!SD.begin(chipSelect)) {  //如果从CS口与SD卡通信失败，串口输出信息Card failed, or not present
        Serial.println("Card failed, or not present");
        // don't do anything more:
        return 1;
  }
     Serial.println("card initialized.");  //与SD卡通信成功，串口输出信息card initialized.     
     digitalWrite(5,HIGH);
     return 0;
}
int get_file_size(char* filename){
    File rfd;
    int buf_len = 0;
    if (!SD.exists(filename)) {
        Serial.println("test.txt doesn't exist.");
        return buf_len;
    }
 
    rfd = SD.open(filename, FILE_READ);
    if(rfd){
        buf_len = rfd.available();
    }
    rfd.close();
    return buf_len;
}

int sd_test(char* filename){
    File rfd;
    const int buf_len = 128;
    char buf[buf_len];
    int rw_count = 0;
 
    if (!SD.exists(filename)) {
        Serial.println("test.txt doesn't exist.");
        return 1;
    }
 
    rfd = SD.open(filename, FILE_READ);
    if (rfd) {
        // read from the file until there's nothing else in it:
        while (rfd.available()) {
            rw_count = rfd.read(buf,buf_len);
            buf[rw_count] = '\0';
            Serial.println(buf);
        }
        // close the file:
        rfd.close();
    } else {
        // if the file didn't open, print an error:
        Serial.print("error opening:");
        Serial.print(filename);
         return 1;
    }
     
    return 0;
}

int play_file(char* filename){
    File rfd;
    int rw_count = 0;
    int8_t data_buffer[DATA_BUFFER_LEN];
    if (!SD.exists(filename)) {
        Serial.println("File to paly doesn't exist.");
        return 0;
    }
 
    rfd = SD.open(filename, FILE_READ);
    if (rfd) {
        // read from the file until there's nothing else in it:
        while (rfd.available()) {
            rw_count = rfd.read(data_buffer,DATA_BUFFER_LEN);
            Vs1003_DATA_Write(data_buffer,rw_count);
        }
        // close the file:
        rfd.close();
    } else {
        // if the file didn't open, print an error:
        Serial.print("error opening:");
        Serial.print(filename);
    }
     
    return 1;
}



void loop() {
   //Mp3Reset();
           Serial.println("yrzf.mp3");
   play_file("yrzf.mp3");
 //  delay(3000);
  // Sintest();
   Serial.println("successful?");

}
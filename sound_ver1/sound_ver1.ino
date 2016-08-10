#include <SPI.h>
#include <SoftwareSerial.h>
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
int fileIndex;
//int fileReadIndex;
/*char* fileList[]={"1.mp3","","","","","",
                "2.mp3"};*/
SoftwareSerial mySerial(2,3); // RX2 TX3;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
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
 
     
     /*File dataFile = SD.open(fileList[0], FILE_READ);  //打开datalog.txt文件
     
      if (dataFile) {
      // while (dataFile.available()) {  //检查是否dataFile是否有数据
       //   Serial.write(dataFile.read());  //如果有数据则把数据发送到串口
       // }
        dataFile.close();  //关闭dataFile
      }
     
      else {
        Serial.println("error opening datalog.txt");  //如果文件无法打开串口发送信息error opening datalog.txt
      } 

      // Serial.println( digitalRead(4));
      */
  
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
       // Serial.println("test.txt doesn't exist.");
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
        Serial.println("File to play doesn't exist.");
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
  //Serial.println("yrzf.mp3");
  //play_file("yrzf.mp3");
  //  delay(3000);
  //   Sintest();
  //Serial.println("successful?");
  
  if(mySerial.available())
  {
   // play_file("1.mp3");
   //  Serial.println(0);
    fileIndex = mySerial.read();
    Serial.println(fileIndex);
    while(mySerial.read() >= 0){}
    if (fileIndex==0)
     { play_file("1.mp3");}
    if (fileIndex==2)
{        play_file("ni.mp3");}
        if (fileIndex==6)
{      play_file("2.mp3");}
    if (fileIndex==8)
{        play_file("jiandao.mp3");}
        if (fileIndex==12)
{      play_file("3.mp3");}
    if (fileIndex==18)
{        play_file("4.mp3");}
        if (fileIndex==20)
{      play_file("miandui.mp3");}
    if (fileIndex==22)
{        play_file("ai.mp3");}
        if (fileIndex==24)
{      play_file("5.mp3");}
    if (fileIndex==26)
{        play_file("bu.mp3");}
        if (fileIndex==28)
{      play_file("bubu.mp3");}
    if (fileIndex==30)
{        play_file("6.mp3");}
        if (fileIndex==32)
{      play_file("yrzf.mp3");}
    if (fileIndex==36)
{        play_file("7.mp3");}
        if (fileIndex==42)
{      play_file("8.mp3");}
   /* if (fileIndex==44)
        play_file("weixiao.mp3");
        if (fileIndex==46)
      play_file("shenghuo.mp3");
    if (fileIndex==48)
        play_file("9.mp3");
        if (fileIndex==50)
      play_file("wan.mp3");
    if (fileIndex==52)
        play_file("shitou.mp3");
        if (fileIndex==54)
      play_file("diao.mp3");
    if (fileIndex==60)
        play_file("wan.mp3");
        if (fileIndex==62)
      play_file("shitou.mp3");
    if (fileIndex==66)
        play_file("high.mp3");*/
   /* switch (fileIndex)
  {
    case 0: play_file("1.mp3"); Serial.println("test"); break;
    case 2: play_file("ni.mp3"); break;
    case 6: play_file("2.mp3"); break;
    case 8: play_file("jiandao.mp3"); break;
    case 12: play_file("3.mp3"); break;
    case 18: play_file("4.mp3"); break;
    case 20: play_file("miandui.mp3"); break;
    case 22: play_file("ai.mp3"); break;
    
    case 24: play_file("5.mp3"); break;
    case 26: play_file("bu.mp3"); break;
    case 28: play_file("bubu.mp3"); break;
    case 30: play_file("6.mp3"); break;
    case 32: play_file("yrzf.mp3"); break;
    case 36: play_file("7.mp3"); break;
    case 42: play_file("8.mp3"); break;
        case 44: play_file("weixiao.mp3"); break;
            case 46: play_file("shenghuo.mp3"); break;
    case 48: play_file("9.mp3"); break;
        case 50: play_file("wan.mp3"); break;
            case 52: play_file("shitou.mp3"); break;
         
    case 54: play_file("diao.mp3"); break;
        case 56: play_file("wo.mp3"); break;
                case 60: play_file("wan.mp3"); break;
            case 62: play_file("shitou.mp3"); break;
            case 66: play_file("high.mp3"); break;
  }*/

      mySerial.write(49); // successful ACK
  }
    else{
      mySerial.write(48); // fail ACK
    
    //Serial.flush();
  }

}

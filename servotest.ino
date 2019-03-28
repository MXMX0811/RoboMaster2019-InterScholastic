#include <Servo.h>
#include <PS2X_lib.h>
#include<Arduino.h>
#define pressures   false
#define rumble      false
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
#define PS2_DAT        A0
#define PS2_CMD        A1
#define PS2_SEL        A2
#define PS2_CLK        A3
PS2X ps2x;
byte vibrate = 0;
const int ServosNum = 5;  //舵机数量
int offset = 8; //舵机引脚开始于8
int HandPosition;
int WristPosition;
int Elbow_1Position;
int Elbow_2Position;
int Elbow_3Position;
int ShoulderPosition;
void PS2_Initial();

Servo servos[ServosNum];
//数组存储电机最大最小位置，经测试得
int servosPos[6][4] = {
  {50, 107, 0, 0},
  {0, 180, 0, 0},
  {0, 180, 0, 0},
  {0, 180, 0, 0},
  {0, 180, 0, 0},
  {0, 180, 0, 0}
};

void setup() {
  PS2_Initial();
  for (int i = 0; i < ServosNum; i++) {
    servos[i].attach(i+offset);
  }
  //舵机初始位置，经测试得到
  servos[0].write(servosPos[0][0]);
  servos[1].write(113);
  servos[2].write(90);
  servos[3].write(120);
  servos[4].write(90);
  servos[5].write(0);
}

void loop() {
  ps2x.read_gamepad(false, vibrate);
  delay(15);
  
  HandPosition = servos[0].read();
  WristPosition = servos[1].read();
  Elbow_1Position = servos[2].read();
  Elbow_2Position = servos[3].read();
  Elbow_3Position = servos[4].read();
  ShoulderPosition = servos[5].read();
  //4、手柄按钮设置请参考PS2库例程，这一步是手柄控制舵机正反转
  if (ps2x.Button(PSB_CROSS)  )
  { 
    Serial.print("hand:");
    Serial.println(HandPosition);
    if (ps2x.Button(PSB_R1) && HandPosition > servosPos[0][0])
      servos[0].write(HandPosition - 1);
    else if (HandPosition < servosPos[0][1])
      servos[0].write(HandPosition + 1);
  }
  if (ps2x.Button(PSB_CIRCLE))
  { 
    Serial.print("wrist:");
    Serial.println(WristPosition);
    if (ps2x.Button(PSB_R1) && WristPosition > servosPos[1][0])
      servos[1].write(WristPosition - 1);
    else if (WristPosition < servosPos[1][1])
      servos[1].write(WristPosition + 1);
  }
  if (ps2x.Button(PSB_SQUARE))
  { 
    Serial.print("elbow_l:");
    Serial.println(Elbow_1Position);
    if (ps2x.Button(PSB_R1) && Elbow_1Position > servosPos[2][0])
      servos[2].write(Elbow_1Position - 1);
    else if (Elbow_1Position < servosPos[2][1])
      servos[2].write(Elbow_1Position + 1);
  }
  if (ps2x.Button(PSB_TRIANGLE))
  { 
    Serial.print("elbow_2:");
    Serial.println(Elbow_2Position);
    if (ps2x.Button(PSB_R1) && Elbow_2Position > servosPos[3][0])
      servos[3].write(Elbow_2Position - 1);
    else if (Elbow_2Position < servosPos[3][1])
      servos[3].write(Elbow_2Position + 1);
  }
  if (ps2x.Button(PSB_L1))
  { 
    Serial.print("elbow_3:");
    Serial.println(Elbow_3Position);
    if (ps2x.Button(PSB_R1) && Elbow_3Position > servosPos[4][0])
      servos[4].write(Elbow_3Position - 1);
    else if (Elbow_3Position < servosPos[4][1])
      servos[4].write(Elbow_3Position + 1);
  }
  if (ps2x.Button(PSB_L2))
  { 
    Serial.print("shoulder:");
    Serial.println(ShoulderPosition);
    if (ps2x.Button(PSB_R1) && ShoulderPosition > servosPos[5][0])
      servos[5].write(ShoulderPosition - 1);
    else if (ShoulderPosition < servosPos[5][1])
      servos[5].write(ShoulderPosition + 1);
  }

}

void PS2_Initial() {
  Serial.begin(57600);
  delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
  int error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  if (error == 0) {
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
    if (pressures)
      Serial.println("true ");
    else
      Serial.println("false");
    Serial.print("rumble = ");
    if (rumble)
      Serial.println("true)");
    else
      Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }
  else if (error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
  else if (error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
  else if (error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  int  type = ps2x.readType();
  switch (type) {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
    case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break;
  }
}

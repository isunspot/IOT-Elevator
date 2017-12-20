#include <Wire.h>
#include <stdio.h>
#define i2caddr 0x04
//definition of arduino pins for motors and ultrasonic sensors
int number = 10;
int MoveUpKey = 52 ;
int MoveDownKey = 53 ;
int BrakeKey = 50 ;
int BrakeReleaseKey = 51 ;
int trig0 = 22;
int echo0 = 23;
int trig1 = 24;
int echo1 = 25;
int trig2 = 26;
int echo2 = 27;
int trig3 = 28;
int echo3 = 29;
//time in ms for which elevator door is open(doorled is lit as there is no mechanical door) 
int doordelay = 3000;
//time for which brake motors are moved
int brakedelay = 250;
//ultrsonic sensor threshold to detect elevator car
int ultrasonic_dur = 220;
int doorled = 40;
int buzzer = 41;
bool emergency = false;

/*
  floors_requests is a boolean array for storing requests of each floor
  directions   0 means lift is stationary
      1 means lift is going up
      2 means lift is going down
*/
int floor_request[4] = {0, 0, 0, 0};
int directions = 0;
int lift_pos = 0;          //current floor the lift is on
int cur_time = 1;          //cur_time increases when lift makes a move

/*
  returns true if the ultrasonic sensor referred by sensorid gives
  a reading of duration less than dur
*/
bool ultrasonic(int sensorid, long dur)
{
  long duration;
  int distance;
  int trig;
  int echo;
  if (sensorid == 0)
  {
    trig = trig0;
    echo = echo0;
  }
  else if (sensorid == 1)
  {
    trig = trig1;
    echo = echo1;
  }
  else if (sensorid == 2)
  {
    trig = trig2;
    echo = echo2;
  }
  else if (sensorid == 3)
  {
    trig = trig3;
    echo = echo3;
  }
  // Clears the trigPin
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  //Serial.print("Distance: ");
  //Serial.println(distance);
  Serial.print("Duration: ");
  Serial.println(duration);
  if (duration < dur)
    return 1;
  else
    return 0;
}
/*
  Elevator Functions

  Functions to move elevator up or down by ONE floor
*/

void open_door()
{
  brake();
  digitalWrite(doorled,HIGH);
  delay(doordelay);
  digitalWrite(doorled,LOW);  
}

//Functions for braking system
void brake()
{
  digitalWrite(BrakeKey , LOW);
  digitalWrite(BrakeReleaseKey , LOW) ;
}
void brake_release()
{
  digitalWrite(BrakeKey , LOW);
  digitalWrite(BrakeReleaseKey , HIGH) ;
}
//Moves the elevator up by one floor till top floor.
void move_up()
{
  if (lift_pos < 3)
  {
    while (!ultrasonic(lift_pos + 1, ultrasonic_dur))
    {
      digitalWrite(MoveUpKey , HIGH);
      digitalWrite(MoveDownKey , LOW);
      if(ultrasonic(3,ultrasonic_dur))
        break;  
    }
    lift_pos += 1;
    if (floor_request[lift_pos] != 0)
    {
      digitalWrite(MoveUpKey , LOW);
      digitalWrite(MoveDownKey , LOW);
      open_door();
      floor_request[lift_pos] = 0;
      //check for further requests . find the first request (first come first serve)
      int min_ind = 0;
      for (int i = 1; i < 4; i++)
      {
        if (floor_request[i] < floor_request[min_ind])
          min_ind = i;
      }
      if (floor_request[min_ind] == 0) //no request
        directions = 0;
      else
      {
        if (min_ind > lift_pos)
        {
          directions = 1;
          brake_release();
        }
        else
        {
          directions = 2;
          brake_release();
        }
      }
    }
  }
  else
  {
    Serial.println("Can't move above top floor");
  }
}
//Moves the elevator down by one floor
void move_down()
{
  if (lift_pos > 0)
  {
    while (!ultrasonic(lift_pos - 1, ultrasonic_dur))
    {
      digitalWrite(MoveUpKey , LOW);
      digitalWrite(MoveDownKey , HIGH);
      if(ultrasonic(0,ultrasonic_dur))
        break;
      
    }
    lift_pos -= 1;
    if (floor_request[lift_pos] != 0)
    {
      digitalWrite(MoveUpKey , LOW);
      digitalWrite(MoveDownKey , LOW);
      open_door();
      floor_request[lift_pos] = 0;
      //check for further requests . find the first request (first come first serve)
      int min_ind = 0;
      for (int i = 1; i < 4; i++)
      {
        if (floor_request[i] < floor_request[min_ind])
          min_ind = i;
      }
      if (floor_request[min_ind] == 0) //no request
        directions = 0;
      else
      {
        if (min_ind > lift_pos)
        {
          directions = 1;
          brake_release();
        }
        else
        {
          directions = 2;
          brake_release();
        }
      }
    }
    
  }
  else
  {
    Serial.println("Can't move below ground floor");
  }
}

//prints the current floor of the lift and the direction it is going
void lift_status()
{
  char buf[100];
  int len = sprintf(buf, "direction %d floor %d time %d", directions, lift_pos, cur_time);
  for (int l = 0; l < len; l++)
    Serial.print(buf[l]);
  Serial.print("\n");
}
//Takes the elevator to ground floor.Used when system restarts
void moveto_ground()
{
  brake_release();
  while (!ultrasonic(0, ultrasonic_dur))
  {
    digitalWrite(MoveUpKey , LOW);
    digitalWrite(MoveDownKey , HIGH) ;
  }
  digitalWrite(MoveUpKey , LOW);
  digitalWrite(MoveDownKey , LOW) ;
  brake();
}

void emergency_stop()
{
  brake_release();
  digitalWrite(buzzer,HIGH);
  digitalWrite(MoveUpKey , LOW);
  digitalWrite(MoveDownKey , LOW);  
}
void emergency_release()
{
  digitalWrite(buzzer,LOW);
}
void setup() {
  pinMode(MoveUpKey , OUTPUT);
  pinMode(MoveDownKey , OUTPUT);
  pinMode(BrakeKey , OUTPUT);
  pinMode(BrakeReleaseKey , OUTPUT);
  pinMode(trig0, OUTPUT);
  pinMode(echo0, INPUT);
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(trig3, OUTPUT);
  pinMode(echo3, INPUT);
  pinMode(doorled, OUTPUT);

  Serial.begin(9600);
  Wire.begin(i2caddr);
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  moveto_ground();
}

// callback for sending data
void sendData() {
  Wire.write(lift_pos);
}
void loop() {

  if (directions == 0)         //lift stationary (no requests)
  { for (int i = 0; i < 4; i++)
    {
      if (floor_request[i] != 0) //if request for some position
      {
        lift_status();
        if (i == lift_pos) //request from current floor
        {
          floor_request[i] = 0;
          open_door();
        }
        else if (i < lift_pos) //go down
        {
          directions = 2;
          brake_release();
        }
        else                  //go up
        {
          directions = 1;
          brake_release();
        }
      }
    }
  }
  else if (directions == 1)      //lift is moving up
  {
    move_up();
    lift_status();
  }
  else if (directions == 2)      //lift is moving down
  {
    move_down();
    lift_status();
  }
  delay(100);
}
// callback for received data
void receiveData(int byteCount) {

  while (Wire.available()) {
    number = Wire.read();
    //Serial.print("data received: ");
    //Serial.println(number);
    if (number == 0) {
      cur_time++;
      if (floor_request[0] == 0)
      {
        floor_request[0] = cur_time;
      }
      Serial.print("Floor request: 0\n");
    } else if (number == 1) {
      cur_time++;
      if (floor_request[1] == 0)
      {
        floor_request[1] = cur_time;
      }
      Serial.print("Floor request: 1\n");
    }
    else if (number == 2) {
      cur_time++;
      if (floor_request[2] == 0)
      {
        floor_request[2] = cur_time;
      }
      Serial.print("Floor request: 2\n");
    }
    else if (number == 3) {
      cur_time++;
      if (floor_request[3] == 0)
      {
        floor_request[3] = cur_time;
      }
      Serial.print("Floor request: 3\n");
    }
    else if (number == 4) {
      emergency = !emergency;
      if(emergency)
        emergency_stop();
      else 
        emergency_release();  
      Serial.print("EMERGENCY\n");
    }
    else if (number == 5) {
      for (int i = 0; i < 4; i++)
      {
        floor_request[i] = 0;
      }
      moveto_ground();
      cur_time = 1;
      lift_pos = 0;
      directions = 0;
      emergency = false;
      Serial.println("Initialising");
    }
  }
}


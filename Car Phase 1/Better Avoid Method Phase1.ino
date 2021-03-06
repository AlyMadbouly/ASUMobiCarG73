
// Motor A
#define pwmLeft 9    // ENA - Enable and PWM
#define leftForward 7  // IN1 - Forward Drive
#define leftReverse 6  // IN2 - Reverse Drive
 
// Motor B
#define pwmRight 3     // ENB - Enable and PWM
#define rightForward 5 // IN3 - Forward Drive
#define rightReverse 4 // IN4 - Reverse Drive

/*ultrasonic*/
#define trig 8
#define echo 2
 
void allStop() {
  digitalWrite(leftForward, LOW);
  digitalWrite(leftReverse, LOW);
  digitalWrite(rightForward, LOW);
  digitalWrite(rightReverse, LOW);
  analogWrite(pwmLeft, 0);
  analogWrite(pwmRight, 0);
}
 
void allForward() {
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftReverse, LOW);
  digitalWrite(rightForward, HIGH);
  digitalWrite(rightReverse, LOW);
  analogWrite(pwmLeft, 255);
  analogWrite(pwmRight, 255);
}
 
void allReverse() {
  digitalWrite(leftForward, LOW);
  digitalWrite(leftReverse, HIGH);
  digitalWrite(rightForward, LOW);
  digitalWrite(rightReverse, HIGH);
  analogWrite(pwmLeft, 255);
  analogWrite(pwmRight, 255);
}
 
void skidsteerLeft() {
  digitalWrite(leftForward, LOW);
  digitalWrite(leftReverse, HIGH);
  digitalWrite(rightForward, HIGH);
  digitalWrite(rightReverse, LOW);
  analogWrite(pwmLeft, 255);
  analogWrite(pwmRight, 255);
}
 
void skidsteerRight() {
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftReverse, LOW);
  digitalWrite(rightForward, LOW);
  digitalWrite(rightReverse, HIGH);
  analogWrite(pwmLeft, 255);
  analogWrite(pwmRight, 255);
}


/*Spin Func to use in avoiding method*/
 void spin() {
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftReverse, LOW);
  digitalWrite(rightForward, LOW);
  digitalWrite(rightReverse, HIGH);
  
  /*This should decrease spinning speed!...I don't know actually*/
  analogWrite(pwmLeft, 150);
  analogWrite(pwmRight, 150);
}
//////////////// End Of Functions //////////////////
 
void setup() {
   Serial.begin(9600);
 
  // Set pins to motor driver (L298) to outputs
  pinMode(pwmLeft, OUTPUT);
  pinMode(leftForward, OUTPUT);
  pinMode(leftReverse, OUTPUT);
  pinMode(pwmRight, OUTPUT);
  pinMode(rightForward, OUTPUT);
  pinMode(rightReverse, OUTPUT);
 /*Ultrasonic Pins*/
  pinMode (trig ,OUTPUT);
  pinMode (echo,INPUT );
}

 /*UltraSonic Readings*/
 long duration,distance;
 long USReadings()
{
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  duration = pulseIn(echo,HIGH);
  distance= (duration/2)*0.034 ; 
  return(distance);
}

void loop() {

  int val = {0};
  if (Serial.available() >0)
  { //take action when a byte is received
      val= Serial.read(); // read the byte
  } 
  Serial.print(USReadings());
  Serial.print("\n");
  long check = USReadings();
  if(check > 20)
  {
    if(val=='F')
     {
     allForward();
     }

     else if(val=='B')
     {
      allReverse(); 
     }

     else if(val=='R')
     {
      skidsteerRight(); 
     }

     else if(val=='L')
     {
      skidsteerLeft();
     }
  
   /////////////////////////////////////////
   /*This Condition is not included in the car yet*/
   
     else if(val=='S')
     {
      allStop();
     }
  }
 int Break_Checker = 0;
  if(check <= 20 && Break_Checker == 0)
  { 
    check = 0;
    allStop();
    delay(500);  
   
    float dist[10] = {0}; /*Array to store readings*/
    float Maximum = 0 ;
   
    spin();
    
    /*for loop to store diff readings after 285 mseconds*/
    
    for(int i = 0 ; i < 10 ; i++)
    {    
      dist[i]= USReadings();
     
     /*Time Interval Between tow diff readings*/
     
      delay(285); 
    }
   
   /*To Get Max Value in compare*/
   
    float compare = dist[0];
    for(int i = 1 ; i < 10 ; i++)
    {
      if (compare < dist[i])
      {
      Maximum = dist[i];  
      }
    }
   
    /**/
   
    for (true ; true ;delay(100))
    {
      if(Maximum <= USReadings())
      {
       allForward(); 
       Break_Checker = 1;
       break;
      }
    }
    
  }

 


delay(100);
}


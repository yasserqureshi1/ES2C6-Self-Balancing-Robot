// ------------------- 02/12/2019 11:10am -----------------

// High Jitter
// High overshoot
// 

 // ----------------- PIN LOCATIONS -------------------
 int pwm1 = 6;  // enable pin EN2
 int pwm2 = 11; // enable pin EN1
 int in_1 = 10;
 int in_2 = 9;
 int in_3 = A5 ;
 int in_4 = 5;

 // ------------------ INITIALISING VARIABLES --------------------
 int currentAngle, currenttime, balance;
 int currenterror, proportional, derivative ;
 int MOTOR, timegap ;
 int up, yaw, pitch, roll ;

 // ------------------ DEFINING VARIABLES -----------------------
 int targetAngle = 580 ;                // INITIALLY 512 BUT CHANGED TO ACCOUNT FOR WEIGTH DISTRIBUTION
 int previoustime = 0 ;
 int previouserror = 0 ;
 int previousAngle = 0 ;
 int integral = 0 ;
 
 int Kp = 38 ;
 int Ki = 1 ;
 float Kd = 12 ;


void setup() {
  Serial.begin(9600);
  // --------------------- SETTING PIN MODES ----------------
  pinMode(in_1, OUTPUT) ;
  pinMode(in_2, OUTPUT) ;
  pinMode(in_3, OUTPUT) ;
  pinMode(in_4, OUTPUT) ;
  pinMode(pwm1, OUTPUT) ;
  pinMode(pwm2, OUTPUT) ;  

  // ----------- INITIALISING STARTING CONIDITIONS ----------
  digitalWrite(in_1, 0) ;
  digitalWrite(in_2, 0) ;
  digitalWrite(in_3, 0) ;
  digitalWrite(in_4, 0) ;

}

void loop() {
  //------------------ TIMER -----------------------
  currenttime = millis();
  timegap = currenttime - previoustime ;
  delay(20) ;             
  
  // ---------------- DATA INPUTS ----------------
  balance = digitalRead(A0);      // UP PIN
  yaw = analogRead(A1);           // - NOT USED
  roll = analogRead(A2) ;         // - NOT USED
  pitch = analogRead(A3);         // ANGLE OF INCLINATION

  currentAngle = pitch ;
  
  if (balance == false)     // -------------- IF NOT WITHIN BALANCING RANGE -> ROBOT IS OFF
  {
    // SET EVERYTHING TO ZERO
    analogWrite(pwm1, 0) ;
    analogWrite(pwm2, 0) ;
    digitalWrite(in_1, 0);
    digitalWrite(in_2, 0);
    digitalWrite(in_3, 0);
    digitalWrite(in_4, 0);
  }

  if (balance == true)    // --------------- IF WITHIN BALANCING RANGE -> ROBOT IS ON
  {
    //  -------- PID CONTROL ------------
    currenterror = currentAngle - targetAngle ; // works
    proportional = currenterror ;
    integral = integral + (currenterror  * timegap) ;
    integral = constrain(integral, -100, 100) ;
    derivative = (currenterror - previouserror) / timegap ;

    MOTOR = (Kp * proportional) + (Ki * integral) + (Kd * derivative) ;

    MOTOR = map(MOTOR, -5400, 5400, -230, 230) ;
    MOTOR = constrain(MOTOR, -255, 255) ;
 

    if (MOTOR < 5 && MOTOR > 90)
      {
        MOTOR = map(MOTOR, 5, 90, 60, 120) ;
      }
    else if (MOTOR > -90 && MOTOR < -5)
    {
        MOTOR = map(MOTOR, -90, -5, -120, -60);
    }


   // -------- WRITING TO MOTORS -----------

    if (currenterror < -150)
    { 
      // FORWARDS (GLASS SIDE)
      digitalWrite(in_1, HIGH);
      digitalWrite(in_2, LOW);
      digitalWrite(in_3, LOW);
      digitalWrite(in_4, HIGH);
      MOTOR = abs(MOTOR) ;
    }

    else if (currenterror > 150)
    {
      // BACKWARDS (WIRES)
      digitalWrite(in_1, LOW);
      digitalWrite(in_2, HIGH);
      digitalWrite(in_3, HIGH);
      digitalWrite(in_4, LOW);
      MOTOR = abs(MOTOR) ;
    }

    else if (currenterror > -150 && currenterror < -2)
    { 
      // FORWARDS (GLASS SIDE)
      digitalWrite(in_1, HIGH);
      digitalWrite(in_2, LOW);
      digitalWrite(in_3, LOW);
      digitalWrite(in_4, HIGH);
      MOTOR = abs(constrain(MOTOR, -180, 180)) ;
    }

    else if (currenterror < 150 && currenterror > 2)
    { 
      // FORWARDS (GLASS SIDE)
      digitalWrite(in_1, LOW);
      digitalWrite(in_2, HIGH);
      digitalWrite(in_3, HIGH);
      digitalWrite(in_4, LOW);
      MOTOR = abs(constrain(MOTOR, -180, 180)) ;
    }

    
    else
    {
      MOTOR = 0 ;
    }

    analogWrite(pwm1, MOTOR);
    analogWrite(pwm2, MOTOR);
    // --------------------------------------

    // ----------- PRINT VALUES --------------
//    Serial.print(" -- PID CONTROL VALUES --- " );
//    Serial.print("currrentangle ");
//    Serial.println(currentAngle);
//    Serial.print("balance ");
//    Serial.println(balance);
//    Serial.print("yaw") ;
//    Serial.println(yaw) ;
//    Serial.print("MOTOR") ;
    Serial.print(MOTOR);
    Serial.print(",") ;
//    Serial.print("CURRENT ANGLE") ;
//    Serial.println(currentAngle) ;
//    Serial.print("PITCH") ;
//    Serial.println(pitch) ;
//    Serial.print("CURRENT ERROR") ;
    Serial.println(currenterror) ;
    Serial.println(" ");   
   // ---------------------------------------


    
  }
}

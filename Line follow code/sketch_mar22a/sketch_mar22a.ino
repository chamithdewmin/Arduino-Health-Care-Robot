#define enA 10  // Enable1 L298 Pin enA 
#define in1 9   // Motor1 L298 Pin in1 
#define in2 8   // Motor1 L298 Pin in2 
#define in3 7   // Motor2 L298 Pin in3 
#define in4 6   // Motor2 L298 Pin in4 
#define enB 5   // Enable2 L298 Pin enB 

#define R_S A0  // IR sensor Right
#define L_S A1  // IR sensor Left

#define TRIG 4  // Ultrasonic sensor Trig pin
#define ECHO 3  // Ultrasonic sensor Echo pin

void setup() {  
    pinMode(R_S, INPUT);  
    pinMode(L_S, INPUT);  

    pinMode(enA, OUTPUT);  
    pinMode(in1, OUTPUT);  
    pinMode(in2, OUTPUT);  
    pinMode(in3, OUTPUT);  
    pinMode(in4, OUTPUT);  
    pinMode(enB, OUTPUT);  

    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    analogWrite(enA, 200); // Set motor speed 
    analogWrite(enB, 200); 
}

void loop() {  
    bool rightDetected = digitalRead(R_S);
    bool leftDetected = digitalRead(L_S);

    int distance = getUltrasonicDistance();

    if (distance <= 20) {  
        Stop();  // Stop motors if obstacle is within 20 cm
    } else if (rightDetected && leftDetected) {  
        allMotorsOn();  // If both IR sensors detect something → Move forward  
    } else if (!rightDetected && !leftDetected) {  
        Stop();  // If both IR sensors detect nothing → Stop
    } else if (!leftDetected) {  
        rightMotorsOn();  // If only the left IR sensor detects nothing → Right-side motors work  
    } else if (!rightDetected) {  
        leftMotorsOn();  // If only the right IR sensor detects nothing → Left-side motors work  
    }
}

void allMotorsOn() {  
    digitalWrite(in1, HIGH);  
    digitalWrite(in2, LOW);  
    digitalWrite(in3, HIGH);  
    digitalWrite(in4, LOW);  
}

void rightMotorsOn() {  
    digitalWrite(in1, HIGH);  
    digitalWrite(in2, LOW);  
    digitalWrite(in3, LOW);  
    digitalWrite(in4, LOW);  
}

void leftMotorsOn() {  
    digitalWrite(in1, LOW);  
    digitalWrite(in2, LOW);  
    digitalWrite(in3, HIGH);  
    digitalWrite(in4, LOW);  
}

void Stop() {  
    digitalWrite(in1, LOW);  
    digitalWrite(in2, LOW);  
    digitalWrite(in3, LOW);  
    digitalWrite(in4, LOW);  
}

// Function to measure distance using the ultrasonic sensor
int getUltrasonicDistance() {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    long duration = pulseIn(ECHO, HIGH);
    int distance = duration * 0.034 / 2; // Convert to cm

    return distance;
}

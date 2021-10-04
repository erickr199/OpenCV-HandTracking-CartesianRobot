/**
Erick Osvaldo Castelzo Guerra - A01273870
Cartesian Robot controlled by serial comunication
MOTOR_0: y axis
MOTOR_1: x axis
MOTOR_2: z axis

Serial comunication input example:
320,320,0
x   y   hand

if(hand==1)  z=0    //hand openend
if(hand==0)  z=1700 //hand closed

Este programa controla un robot caretsiano el cual funciona con 3 stepper motors. Cada motor modifica una
de las coordenadas de los ejes x,y,z. en la variable 'location' se guarda la coordenada actual y en 
'newLocation' la nueva coordenada que se busca alcanzar. El programa puede recibir comandos por comunicacion
serial. Una vez recive un comando en el formato x,y,hand actualiza la variable 'newLocation' para que
el robot se dirija a la nueva pocision
Los pines utilizados para este programa son
MOTOR_0:         pin 2, 3, 4, 5
MOTOR_1:         pin 6, 7, 8, 9
MOTOR_2:         pin 10,11,12,13
Shutdown button: pin A0

El delay minimo posible para el funcionamiento de los stepper motors es de 2ms, en nuestro caso ocupamos 3ms
*/


#define MOTOR_0 2
#define MOTOR_1 6
#define MOTOR_2 10
#define BUTTON  A0

//funciton declarations
void OneStep(bool dir, int motor);
void move_x();
void move_y();
void move_z();
void reach_location();
void stop_all();


//Global varibles
int step_number[3] = {0,0,0};//variable that stores the step number for each motor, used in OneStep() function

//Estructure to store coordinates
struct coords{
  int x;
  int y;
  int z;
};
//current location
coords location    = {0,0,0};
//New location        x y z
coords newLocation = {0,0,0};


void setup() {  
  //Motor Pin inicialisation
  pinMode(MOTOR_0, OUTPUT);
  pinMode(MOTOR_0 + 1, OUTPUT);
  pinMode(MOTOR_0 + 2, OUTPUT);
  pinMode(MOTOR_0 + 3, OUTPUT);

  pinMode(MOTOR_1, OUTPUT);
  pinMode(MOTOR_1 + 1, OUTPUT);
  pinMode(MOTOR_1 + 2, OUTPUT);
  pinMode(MOTOR_1 + 3, OUTPUT);

  pinMode(MOTOR_2, OUTPUT);
  pinMode(MOTOR_2 + 1, OUTPUT);
  pinMode(MOTOR_2 + 2, OUTPUT);
  pinMode(MOTOR_2 + 3, OUTPUT);

  //Shutdown button
  pinMode(A0, INPUT_PULLUP);

  //Serial comunication
  Serial.begin(9600);
  Serial.setTimeout(5);//default: 1000, changed to run in realtime
  delay(30);
}

int pos;
String cad,cad1,cad2,cad3;

//MAIN FUNCTION
void loop() {
//  newLocation={-2000,-2000,-2000};
//  reach_location();
//  while(1);

  //Check if serial command was recived
  if(Serial.available()){
    cad = Serial.readString();
    //Separar en substrings al encontrar ','
    pos  = cad.indexOf(',');
    cad1 = cad.substring(0,pos);
    cad2 = cad.substring(pos+1); 
    pos = cad2.indexOf(',');
    cad3 = cad2.substring(pos+1);
    cad2 = cad2.substring(0,pos);

    //Assign new Coordinates
    int x = cad1.toInt()*6;
    int y = cad2.toInt()*6;
    if(abs(newLocation.x - x)>32) newLocation.x = x;//x coord
    if(abs(newLocation.y - y)>32) newLocation.y = y;//y coord 
    if(cad3.toInt() == 1) newLocation.z=0;          //z coord(hand open)
    else                  newLocation.z=1700;       //z coord(hand closed)
    //reach_location();
  }

  //Update location
  move_x();
  move_y();
  move_z();
  delay(3);

  //Shutdwon button
  if(digitalRead(A0) == LOW){ 
    stop_all();
  } 
}



////  FUNCTIONS  ////
//Move a steper motor one step
void OneStep(bool dir, int motor){
  if(dir){  //GO RIGHT//
    switch(step_number[motor/4]){
      case 0:
        digitalWrite(motor + 0, HIGH);
        digitalWrite(motor + 1, LOW);
        digitalWrite(motor + 2, LOW);
        digitalWrite(motor + 3, LOW);
        break;
      case 1:
        digitalWrite(motor + 0, LOW);
        digitalWrite(motor + 1, HIGH);
        digitalWrite(motor + 2, LOW);
        digitalWrite(motor + 3, LOW);
        break;
      case 2:
        digitalWrite(motor + 0, LOW);
        digitalWrite(motor + 1, LOW);
        digitalWrite(motor + 2, HIGH);
        digitalWrite(motor + 3, LOW);
        break;
      case 3:
        digitalWrite(motor + 0, LOW);
        digitalWrite(motor + 1, LOW);
        digitalWrite(motor + 2, LOW);
        digitalWrite(motor + 3, HIGH);
        break;
    }
  }
  else{  //GO LEFT//
     switch(step_number[motor/4]){
      case 0:
        digitalWrite(motor + 0, LOW);
        digitalWrite(motor + 1, LOW);
        digitalWrite(motor + 2, LOW);
        digitalWrite(motor + 3, HIGH);
        break;
      case 1:
        digitalWrite(motor + 0, LOW);
        digitalWrite(motor + 1, LOW);
        digitalWrite(motor + 2, HIGH);
        digitalWrite(motor + 3, LOW);
        break;
      case 2:
        digitalWrite(motor + 0, LOW);
        digitalWrite(motor + 1, HIGH);
        digitalWrite(motor + 2, LOW);
        digitalWrite(motor + 3, LOW);
        break;
      case 3:
        digitalWrite(motor + 0, HIGH);
        digitalWrite(motor + 1, LOW);
        digitalWrite(motor + 2, LOW);
        digitalWrite(motor + 3, LOW);
        break;
    }
  }

  step_number[motor/4]++;
  if(step_number[motor/4] > 3) step_number[motor/4] =0;
}


//move 'y' direction
void move_y(){
  if(location.y < newLocation.y){
    OneStep(true, MOTOR_0);
    location.y++;
  }
  else if(location.y > newLocation.y){
    OneStep(false, MOTOR_0);
    location.y--;
  }

}

//move 'x' direction
void move_x(){
  if(location.x < newLocation.x){
    OneStep(false, MOTOR_1);
    location.x++;
  }
  else if(location.x > newLocation.x){
    OneStep(true, MOTOR_1);
    location.x--;
  }
}

//move 'z' direction
void move_z(){ 
  if(location.z < newLocation.z){
    OneStep(true, MOTOR_2);
    location.z++;
  }
  else if(location.z > newLocation.z){
    OneStep(false, MOTOR_2);
    location.z--;
  }
}

//Reach new coordinate Location
void reach_location(){
  while((location.x != newLocation.x) || (location.y != newLocation.y) || (location.z != newLocation.z)){
    move_x();
    move_y();
    move_z();
    delay(3);
  }
}

//Finshish execution
void stop_all(){
  //Go to starting configuration
  newLocation={0,0,0};
  reach_location();


  //Shutdown motors
  //motor0
   digitalWrite(MOTOR_0, LOW);
   digitalWrite(MOTOR_0 + 1, LOW);
   digitalWrite(MOTOR_0 + 2, LOW);
   digitalWrite(MOTOR_0 + 3, LOW);
   //motor1
   digitalWrite(MOTOR_1, LOW);
   digitalWrite(MOTOR_1 + 1, LOW);
   digitalWrite(MOTOR_1 + 2, LOW);
   digitalWrite(MOTOR_1 + 3, LOW);
  //motor2
   digitalWrite(MOTOR_2, LOW);
   digitalWrite(MOTOR_2 + 1, LOW);
   digitalWrite(MOTOR_2 + 2, LOW);
   digitalWrite(MOTOR_2 + 3, LOW);
   
   //infinite loop
   while(1);
  
}

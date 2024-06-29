Written by [redacted]
# Infra Red program use
For communication between Arduino's a program is set up for use with infra red LED's and sensors. <br>
In this report you'll find how the program works, what resources it uses and how the protocol works.
## How the program works
The program works by initializing it ones ready. <br>
This is done using the public methode:
```c++
void InfraRed::init(void){
  DDRD |= (1<<DDD6);
  timer0_init();
  timer1_init();
  sei();
}
```
This initializes the used timers, set pin ```D6``` as output and enables global interupts. <br>
This methode is to be called ones an object has been made of the ```InfraRed``` class or the code has to be edited for using constructors. <br> <br>
Ones initialize the program will automatically look for received messages <br>
These are to be added to an interupt routine. <br> <br>
Ones initialize the program has been set to defualt 56Khz. <br>
If the other Arduino uses 38Khz instead the following methode is suppose to be called:
```c++
void InfraRed::set_hertz(bool value){
  if (value){
    IRCOMP = IRCOMPSLOW;
    IRCOUN = IRCOUNSLOW;
  }
  else{
    IRCOMP = IRCOMPFAST;
    IRCOUN = IRCOUNFAST;
  }
}
```
If the value ```true``` is used the signal used will be set to 38Khz, using the value ```false``` will revert it back to 56Khz. <br> <br>
Lastly a message can be send at any time as long as these is currently not a message in transition. <br>
To send a message over infra red the following methode is to be called:
```c++
void InfraRed::send_message(unsigned short int adress, unsigned short int mes){
  if (!sending){
    message = parse_send_message(adress, mes);
    sending = true;
  }
}
```
This requires the person have an adress and data to send. <br>
These will be parsed within' the class and send at the next available moment. <br>
The sending of a message takes time so messages are not suppose to be send directly after one another.
## Used resources
The infra red library currently used timer0 and timer1 of the Arduino. <br>
Timer0 is being used for generating a constant signal that the other Arduino can pick up with the infra red sensor.
```c++
ISR(TIMER0_COMPA_vect){
  static unsigned int timer = 0;
  if (timer >= IRCOUN){
    PORTD ^= (1<<PORTD6);
    timer = 0;
  }
  else
    timer++;
}
```
Timer1 is used to consitantly send and read messages over the infra red LED and sensor. <br>
This is used to make sure both Arduinos read at the same speed as they write, this way almost no timer related errors are created.
```c++
ISR(TIMER1_COMPA_vect){
  static unsigned int timer = 0;
  if (timer >= SIGCOUN){
    send_message();
    read_messsage();
    timer = 0;
  }
  else
    timer++;
}
```
## How the protocol works
A message send over infra red uses a custom protocol that has been made simple to understand. <br>
*Because this still is a student project, no real world are expected from this library* <br>
The whole message consist of 2 bytes, 16 bits, these look as follows: ```1101100110101101``` <br>
These are build up of 5 parts.
-> Initialize bits <br>
-> adress bits <br>
-> first parity bit <br>
-> data bits <br>
-> second partiy bit <br> <br>
Breaking this up from the previous show message it looks like this: ```11 0110 0 11010110 1``` <br>
The initialize bits are there to tell the other Arduino that a message is being send. <br>
The first parity bit is there to error correct the given adress. <br>
The second parity bit is there to do the same but for the data bits. <br>
This message is send from left to right over the infra red LED.
## Adress code use
Each transmitted message includes a 4-bit adress which tells the Arduino what it needs to do. <br>
Each adress code is assigned as follows: <br>
| Adress code | Action description | Requires given data | <br>
|---------------|--------------------------|---------------------| <br>
|-```0x0```---------|-Not assigned---------|-No------------------| <br>
|-```0x1```---------|-Send highscore-------|-Yes-----------------| <br>
|-```0x2```---------|-Send lines-----------|-Yes-----------------| <br>
|-```0x3```---------|-Send Score-----------|-Yes------------------| <br>
|-```0x4```---------|-Not assigned---------|-No------------------| <br>
|-```0x5```---------|-Not assigned---------|-No------------------| <br>
|-```0x6```---------|-Not assigned---------|-No------------------| <br>
|-```0x7```---------|-Not assigned---------|-No------------------| <br>
|-```0x8```---------|-Not assigned---------|-No------------------| <br>
|-```0x9```---------|-Not assigned---------|-No------------------| <br>
|-```0xA```---------|-Not assigned---------|-No------------------| <br>
|-```0xB```---------|-Not assigned---------|-No------------------| <br>
|-```0xC```---------|-Not assigned---------|-No------------------| <br>
|-```0xD```---------|-Send game over-------|-No------------------| <br>
|-```0xE```---------|-Send start game------|-No------------------| <br>
|-```0xF```---------|-Send acknowledgement-|-No------------------|
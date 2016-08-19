class L293d{
 public:
 L293d(byte enable,byte in1,byte in2){
  this->enable=enable;
  this->in1=in1;
  this->in2=in2;
  pinMode(enable,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
}
void go(bool dir, byte pwm){
  digitalWrite(in1,dir);
  digitalWrite(in2,!dir);
  analogWrite(enable,pwm);
}
void stop(){
  analogWrite(enable,0);
}
 private:
 byte enable,in1,in2; 
};



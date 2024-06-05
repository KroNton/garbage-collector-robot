
int obstaclePin = 10;
bool hasObstacle = false; // LOW MEANS NO OBSTACLE

void ir_setup() {
pinMode(LEDpin, OUTPUT);
pinMode(obstaclePin, INPUT);
}

bool ir_obstacle() {
hasObstacle = digitalRead(obstaclePin);

return hasObstacle;
}


int obstaclePin = 10;
int hasObstacle = LOW; // LOW MEANS NO OBSTACLE

void ir_setup() {
pinMode(obstaclePin, INPUT);
}

int ir_obstacle() {
hasObstacle = digitalRead(obstaclePin);

return hasObstacle;
}

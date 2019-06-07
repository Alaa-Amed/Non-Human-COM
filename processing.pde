ArrayList<Particle> particles;
float numParticles =100;
float minDistance =300;
import processing.video.*;
import processing.serial.Serial;
Serial myPort; 
float red = 255;
float blue = 0;
float green = 0;
String val ="hey";

float min = 100;
public void setup() {
  fullScreen();  
  //size(1000, 800);
    background(20);
    smooth();
    String portName = Serial.list()[1]; 
    myPort = new Serial(this, portName, 9600);
    min = 100;
    
    particles = new ArrayList();
    //numParticles = 100;
    //minDistance = 300;

    for (int i = 0; i < numParticles; i++) {
        Particle p = new Particle(random(min, width - min), random(min, height - min), 5); 
        particles.add(p);
        p.update();
    }
}

void mouseClicked() {
    setup();
}

void draw() {
    background(20);

    for (Particle p1 : particles) {
        for (Particle p2 : particles) {
            if (p1 != p2) {
                float distance = dist(p1.x, p1.y, p2.x, p2.y);
              
                if (distance < minDistance) {
                    stroke(color(red, blue, green), 255-((distance/minDistance)*255));
                    strokeWeight(1);
                    line(p1.x, p1.y, p2.x, p2.y);
                }
            }
        }
    }
    
    for (Particle p : particles) {
        p.draw();
        p.update();
    }
}
class Particle {
    float px, py;
    float x, y;
    float rx, ry;
    float rT;
    float size;
    
    Particle(float x, float y, float size) {
        this.px = x;
        this.py = y;
        this.size = size;
        this.rx = random(blue, 100);
        this.ry = this.rx;
        if (random(100) < 50) {
            rx *= -1;
        }
        this.rT = random(green, 5000);
    }
    
    void update() {
        float t = millis()/rT;
        x = (int)(px + rx * cos(t));
        y = (int)(py + ry * sin(t));
    }
    
    void draw() {
        noStroke();
        fill(200);
        ellipse(x, y, size, size);
    }
}

void serialEvent(Serial myPort){
  val = myPort.readStringUntil('\n');
 if (val != null) {
    float inB = float(val);
    red = inB;
    
    if (float(val) > 124) {
      blue = inB*random(30);
    } else {
      blue = inB *2;
    }
    
    if (float(val) > 83) {  
      green = inB*random(24);
    } else {
      green = inB *3;
    }
      
    numParticles = inB*2;
    minDistance = inB*4;
    min = random(200)-9;
    
 }
  
}

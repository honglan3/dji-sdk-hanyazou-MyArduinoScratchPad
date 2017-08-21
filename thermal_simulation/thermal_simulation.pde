float k0 = 0.1;
float k1 = 0.5;
float k2 = 0.1;
float[] t0;
float[] t1;
float tEnv = 28.0;
PImage img;
PFont f;
int N = 3;
int NELEMS = N*3+2;
int DURATION = 600;
int MAX_TEMP = 250;
int TEARMO_IMAGE_HEIGHT = 50;
int sensor_pos = N;
int heat_source = N*2+1;
int cur_time;
boolean heater_on;
boolean prev_heater_on;

void setup() {
  t0 = new float[NELEMS];
  t1 = new float[NELEMS];
  for (int i = 0; i < NELEMS; i++) {
    t1[i] = tEnv;
  }
  img = createImage(NELEMS, 1, RGB);
  f = createFont("Arial",16,true);
  //colorMode(HSB, 255*100/68, 100, 100);
  cur_time = 0;

  size(1280, 400);
  background(255);
  stroke(200);
  temp_line(100, 0, 100, 250);
  temp_line(200, 0, 200, 250);
  temp_line(300, 0, 300, 250);
  temp_line(400, 0, 400, 250);
  temp_line(500, 0, 500, 250);
  temp_line(0, 50, DURATION, 50);
  temp_line(0, 100, DURATION, 100);
  stroke(255, 0, 255);
  temp_line(0, 150, DURATION, 150);
  temp_line(0, 180, DURATION, 180);
  temp_line(0, 200, DURATION, 200);
  noSmooth();
  frameRate(60);
}

void temp_line(int x0, int y0, int x1, int y1) {
  line(x0*width/DURATION, height - (y0 * (height-TEARMO_IMAGE_HEIGHT))/MAX_TEMP,
       x1*width/DURATION, height - (y1 * (height-TEARMO_IMAGE_HEIGHT))/MAX_TEMP);
}

void draw() {
  update();

  for (int i=0; i<NELEMS; i++) {
    img.pixels[i] = color(t1[i], 50, 255-t1[i]);
  }
  img.updatePixels();
  image(img, 0, 0, width, TEARMO_IMAGE_HEIGHT);

  textFont(f,16);
  fill(255);
  stroke(255);
  rect(0, TEARMO_IMAGE_HEIGHT, 200, 20);
  String msg = "Time: " + cur_time + "sec Heater: " + (heater_on ? "ON" : "OFF");
  fill(0);
  text(msg,10,TEARMO_IMAGE_HEIGHT+20);

  stroke(0, 0, 255);
  temp_line(cur_time-1, (int)t0[sensor_pos], cur_time, (int)t1[sensor_pos]);
  stroke(255, 0, 0);
  temp_line(cur_time-1, prev_heater_on ? 15 : 5, cur_time, heater_on ? 15 : 5);
}

void update()
{
  for (int i=0; i<NELEMS; i++) {
    t0[i] = t1[i];
  }
  prev_heater_on = heater_on;

  if (cur_time < DURATION-1)
    cur_time++;
  heater_on = false;
  // heat source
  if (cur_time < 50) {
  } else
  if (cur_time < 130) {
    heater_on = true;
  } else
  if (cur_time < 250) {
    if (cur_time % 3 != 0)
      heater_on = true;
  } else
  if (cur_time < 280) {
    heater_on = true;
  } else
  if (cur_time < 300) {
    if (cur_time % 3 != 0)
      heater_on = true;
  }
  if (heater_on) {
    t0[heat_source] += 15;
  }

  int i;
  t1[0] = t0[0] + k0 * (tEnv - 2 * t0[0] + t0[1]);
  for (i=1; i<sensor_pos; i++) {
    t1[i] = t0[i] + k0 * (t0[i-1] - 2 * t0[i] + t0[i+1]);
  }
  t1[i] = t0[i] + k0 * (t0[i-1] - t0[i]) + k1 * (t0[i+1] - t0[i]);
  for (i++; i<heat_source; i++) {
    t1[i] = t0[i] + k1 * (t0[i-1] - 2 * t0[i] + t0[i+1]);
  }
  t1[i] = t0[i] + k1 * (t0[i-1] - t0[i]) + k2 * (t0[i+1] - t0[i]);
  for (i++; i<NELEMS-1; i++) {
    t1[i] = t0[i] + k2 * (t0[i-1] - 2 * t0[i] + t0[i+1]);
  }
  t1[NELEMS-1] = t0[NELEMS-1] + k2 * (t0[NELEMS-2] - 2 * t0[NELEMS-1] + tEnv);
}
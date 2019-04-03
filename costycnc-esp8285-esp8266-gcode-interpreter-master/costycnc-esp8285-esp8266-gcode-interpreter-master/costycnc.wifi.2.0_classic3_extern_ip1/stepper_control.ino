#define max1(a,b) ((a)>(b)?(a):(b))
//init our variables
long max_delta;
long x_counter;
long y_counter;
long z_counter;
bool x_can_step;
bool y_can_step;
bool z_can_step;
int milli_delay;

int costyx=1;
int costyy=1;

void init_steppers()
{
	//turn them off to start.
	disable_steppers();
	
	//init our points.
	current_units.x = 0.0;
	current_units.y = 0.0;
	current_units.z = 0.0;
	target_units.x = 0.0;
	target_units.y = 0.0;
	target_units.z = 0.0;
	
	//pinMode(X_STEP_PIN, OUTPUT);
	//pinMode(X_DIR_PIN, OUTPUT);
	//pinMode(X_ENABLE_PIN, OUTPUT);
	//pinMode(X_MIN_PIN, INPUT);
	//pinMode(X_MAX_PIN, INPUT);
	
	//pinMode(Y_STEP_PIN, OUTPUT);
	//pinMode(Y_DIR_PIN, OUTPUT);
	//pinMode(Y_ENABLE_PIN, OUTPUT);
	//pinMode(Y_MIN_PIN, INPUT);
	//pinMode(Y_MAX_PIN, INPUT);
	
	//pinMode(Z_STEP_PIN, OUTPUT);
	//pinMode(Z_DIR_PIN, OUTPUT);
	//pinMode(Z_ENABLE_PIN, OUTPUT);
	//pinMode(Z_MIN_PIN, INPUT);
	//pinMode(Z_MAX_PIN, INPUT);
	
	//figure our stuff.
	calculate_deltas();
}

void dda_move(long micro_delay)
{
	//enable our steppers
	//digitalWrite(X_ENABLE_PIN, HIGH);
	//digitalWrite(Y_ENABLE_PIN, HIGH);
	//digitalWrite(Z_ENABLE_PIN, HIGH);
	
	//figure out our deltas
	max_delta = max1(delta_steps.x, delta_steps.y);
	max_delta = max1(delta_steps.z, max_delta);

	//init stuff.
	long x_counter = -max_delta/2;
	long y_counter = -max_delta/2;
	long z_counter = -max_delta/2;
	
	//our step flags
	bool x_can_step = 0;
	bool y_can_step = 0;
	bool z_can_step = 0;
	
	if (micro_delay >= 16383)
		milli_delay = micro_delay / 1000;
	else
		milli_delay = 0;

	//do our DDA line!
	do
	{
  if(!max_delta)
break;
		x_can_step = can_step(current_steps.x, target_steps.x, x_direction);
		y_can_step = can_step(current_steps.y, target_steps.y, y_direction);
		z_can_step = can_step(current_steps.z, target_steps.z, z_direction);

		if (x_can_step)
		{
			x_counter += delta_steps.x;
      
			
			if (x_counter > 0)
			{
				//do_step(X_STEP_PIN);
				x_counter -= max_delta;
				
				if (x_direction){
					current_steps.x++;
          costyx=costyx+1;
          if (costyx > 8) costyx=1;
    
}
				else{
					current_steps.x--;     
          costyx=costyx-1;
          if (costyx < 1) costyx=8;
     }
    }
		}

		if (y_can_step)
		{
			y_counter += delta_steps.y;
			
			if (y_counter > 0)
			{
				//do_step(Y_STEP_PIN);
				y_counter -= max_delta;

				if (y_direction){
					current_steps.y++;
          costyy=costyy+1;
          if (costyy > 8) costyy=1;
     }
				else{
					current_steps.y--;
          costyy=costyy-1;
          if (costyy < 1) costyy=8;
      }
      
  	}
		}
		
		if (z_can_step)
		{
			z_counter += delta_steps.z;
			
			if (z_counter > 0)
			{
				//do_step(Z_STEP_PIN);
				z_counter -= max_delta;
				
				if (z_direction)
					current_steps.z++;
				else
					current_steps.z--;
			}
		}
   yield();
     if (costyy==1) {digitalWrite(D5, HIGH);digitalWrite(D6, LOW);digitalWrite(D7, LOW);digitalWrite(D8, LOW);}//  PORTD=0B100000;     
     if (costyy==2) {digitalWrite(D5, HIGH);digitalWrite(D6, HIGH);digitalWrite(D7, LOW);digitalWrite(D8, LOW);}//  PORTD=0B110000;    
     if (costyy==3) {digitalWrite(D5, LOW);digitalWrite(D6, HIGH);digitalWrite(D7, LOW);digitalWrite(D8, LOW);}//  PORTD=0B010000;   
     if (costyy==4) {digitalWrite(D5, LOW);digitalWrite(D6, HIGH);digitalWrite(D7, HIGH);digitalWrite(D8, LOW);}//  PORTD=0B011000;
     if (costyy==5) {digitalWrite(D5, LOW);digitalWrite(D6, LOW);digitalWrite(D7, HIGH);digitalWrite(D8, LOW);}//  PORTD=0B001000;
     if (costyy==6) {digitalWrite(D5, LOW);digitalWrite(D6, LOW);digitalWrite(D7, HIGH);digitalWrite(D8, HIGH);}//  PORTD=0B001100;
     if (costyy==7) {digitalWrite(D5, LOW);digitalWrite(D6, LOW);digitalWrite(D7, LOW);digitalWrite(D8, HIGH);}//  PORTD=0B000100;
     if (costyy==8) {digitalWrite(D5, HIGH);digitalWrite(D6, LOW);digitalWrite(D7, LOW);digitalWrite(D8, HIGH);}//  PORTD=0B100100;

     if (costyx==1) {digitalWrite(D0, HIGH);digitalWrite(D1, LOW);digitalWrite(D2, LOW);digitalWrite(D3, LOW);}//  PORTD=0B100000;     
     if (costyx==2) {digitalWrite(D0, HIGH);digitalWrite(D1, HIGH);digitalWrite(D2, LOW);digitalWrite(D3, LOW);}//  PORTD=0B110000;    
     if (costyx==3) {digitalWrite(D0, LOW);digitalWrite(D1, HIGH);digitalWrite(D2, LOW);digitalWrite(D3, LOW);}//  PORTD=0B010000;   
     if (costyx==4) {digitalWrite(D0, LOW);digitalWrite(D1, HIGH);digitalWrite(D2, HIGH);digitalWrite(D3, LOW);}//  PORTD=0B011000;
     if (costyx==5) {digitalWrite(D0, LOW);digitalWrite(D1, LOW);digitalWrite(D2, HIGH);digitalWrite(D3, LOW);}//  PORTD=0B001000;
     if (costyx==6) {digitalWrite(D0, LOW);digitalWrite(D1, LOW);digitalWrite(D2, HIGH);digitalWrite(D3, HIGH);}//  PORTD=0B001100;
     if (costyx==7) {digitalWrite(D0, LOW);digitalWrite(D1, LOW);digitalWrite(D2, LOW);digitalWrite(D3, HIGH);}//  PORTD=0B000100;
     if (costyx==8) {digitalWrite(D0, HIGH);digitalWrite(D1, LOW);digitalWrite(D2, LOW);digitalWrite(D3, HIGH);}//  PORTD=0B100100;
     
		
		//extruder_manage_temperature();
	
		//wait for next step.
		if (milli_delay > 0)
			{
			  delay(milli_delay);
			}		
		else
			{
			  delayMicroseconds(micro_delay);
			}
	}
	while (x_can_step || y_can_step || z_can_step);
	 
	//set our points to be the same
 
	current_units.x = target_units.x;
	current_units.y = target_units.y;
	current_units.z = target_units.z;
	calculate_deltas();
}

bool can_step(long current, long target, byte direction)
{
	//stop us if we're on target
	if (target == current)
		return false;
  else
	return true;
}

void do_step(byte step_pin)
{
	digitalWrite(step_pin, HIGH);
	delayMicroseconds(5);
	digitalWrite(step_pin, LOW);
}

bool read_switch(byte pin)
{
	//dual read as crude debounce
	
	if ( SENSORS_INVERTING )
		return !digitalRead(pin) && !digitalRead(pin);
	else
		return digitalRead(pin) && digitalRead(pin);
}

long to_steps(float steps_per_unit, float units)
{
	return steps_per_unit * units;
}

void set_target(float x, float y, float z)
{
	target_units.x = x;
	target_units.y = y;
	target_units.z = z;
	
	calculate_deltas();
}

void set_position(float x, float y, float z)
{
	current_units.x = x;
	current_units.y = y;
	current_units.z = z;
	
	calculate_deltas();
}

void calculate_deltas()
{
	//figure our deltas.
	delta_units.x = fabs(target_units.x - current_units.x);
	delta_units.y = fabs(target_units.y - current_units.y);
	delta_units.z = fabs(target_units.z - current_units.z);
Serial.print("int X_STEPS_PER_MM=");
Serial.println(X_STEPS_PER_MM);
				
	//set our steps current, target, and delta
	current_steps.x = to_steps(x_units, current_units.x);
	current_steps.y = to_steps(y_units, current_units.y);
	current_steps.z = to_steps(z_units, current_units.z);

	target_steps.x = to_steps(x_units, target_units.x);
	target_steps.y = to_steps(y_units, target_units.y);
	target_steps.z = to_steps(z_units, target_units.z);

	delta_steps.x = fabs(target_steps.x - current_steps.x);
	delta_steps.y = fabs(target_steps.y - current_steps.y);
	delta_steps.z = fabs(target_steps.z - current_steps.z);
	
	//what is our direction
	x_direction = (target_units.x >= current_units.x);
	y_direction = (target_units.y >= current_units.y);
	z_direction = (target_units.z >= current_units.z);

	//set our direction pins as well
	//digitalWrite(X_DIR_PIN, x_direction);
	//digitalWrite(Y_DIR_PIN, y_direction);
	//digitalWrite(Z_DIR_PIN, z_direction);
}


long calculate_feedrate_delay(float feedrate)
{
	//how long is our line length?
	float distance = sqrt(delta_units.x*delta_units.x + delta_units.y*delta_units.y + delta_units.z*delta_units.z);
	long master_steps = 0;
	
	//find the dominant axis.
	if (delta_steps.x > delta_steps.y)
	{
		if (delta_steps.z > delta_steps.x)
			master_steps = delta_steps.z;
		else
			master_steps = delta_steps.x;
	}
	else
	{
		if (delta_steps.z > delta_steps.y)
			master_steps = delta_steps.z;
		else
			master_steps = delta_steps.y;
	}

	//calculate delay between steps in microseconds.  this is sort of tricky, but not too bad.
	//the formula has been condensed to save space.  here it is in english:
	// distance / feedrate * 60000000.0 = move duration in microseconds
	// move duration / master_steps = time between steps for master axis.
//Serial.println("delta_steps.x");
//Serial.println(delta_steps.x);
//Serial.println("delta_steps.y");
//Serial.println(delta_steps.y);

//Serial.println("delta_units.x");
//Serial.println(delta_units.x);
//Serial.println("delta_units.y");
//Serial.println(delta_units.y);

 
//Serial.println("((0 * 60000000.0) / 0) / 0");
//Serial.println(((0 * 60000000.0) / 0) / 0);
//Serial.println("((0 * 60000000.0) / 0) / 0");
	return ((distance * 60000000.0) / feedrate) / master_steps;	
}

long getMaxSpeed()
{
	if (delta_steps.z > 0)
		return calculate_feedrate_delay(FAST_Z_FEEDRATE);
	else
		return calculate_feedrate_delay(FAST_XY_FEEDRATE);
}

void disable_steppers()
{

     digitalWrite(D0, LOW);digitalWrite(D1, LOW);digitalWrite(D2, LOW);digitalWrite(D3, LOW);
     digitalWrite(D5, LOW);digitalWrite(D6, LOW);digitalWrite(D7, LOW);digitalWrite(D8, LOW);
//  PORTC=0;
//  PORTD=0;
	//enable our steppers
	//digitalWrite(X_ENABLE_PIN, LOW);
	//digitalWrite(Y_ENABLE_PIN, LOW);
	//digitalWrite(Z_ENABLE_PIN, LOW);
     Serial.println("facut");
  
}

module status_board()
{
	union()
	{
		color([0.8,0.8,0.2])  cube(size = [24,20,2], center=true);	
		color([0.5,0.5,0.5])  translate(v=[0, 5, -4]) cube(size = [15,7,8], center=true);	
		color([0.5,0.5,0.5])  translate(v=[0, 5, 5]) cube(size = [5,4,8], center=true);	
		color([0.0,0.5,0.0])  translate(v=[0, -4, 1]) cylinder(h=8, r=2.5);
	}
}

module battery_holder()
{
	difference()
	{
		color([0.5,0.5,0.5])  cube(size = [41,20,17], center=true);	
		cube(size = [37,16,13], center=true);	
		translate(v=[0, 0, 5])  cube(size = [37,22,13], center=true);	
	}
	color([1,1,1]) translate(v=[19.5, 0, 2]) cube(size = [1,4,27], center=true);	
	color([1,1,1]) translate(v=[-19.5, 0, 2]) cube(size = [1,4,27], center=true);
}

module main_circuit()
{
	 union()
	{
		// Bottom layer
		color([0.5,0.8,0.5]) cube(size = [69,53,2], center=true);
		color([0.5,0.5,0.5]) translate(v=[9, 24.1, 5])  cube(size = [43,3,12], center=true);
		color([0.5,0.5,0.5]) translate(v=[15, -24.1, 5])  cube(size = [31,3,12], center=true);
		color([1,1,1]) translate(v=[-30, -2, 5])   cube(size = [10,6,8], center=true);
		color([1,1,1]) translate(v=[-28, 28, 5])   cube(size = [5,4,10], center=true);

		// Camera layer
		color([0.8,0.5,0.5]) translate(v=[0, 0, 13]) cube(size = [68,53,2], center=true);

		color([0.5,0.5,0.5]) translate(v=[9, 24.1, 18])  cube(size = [43,3,12], center=true);
		color([0.5,0.5,0.5]) translate(v=[15, -24.1, 18])  cube(size = [31,3,12], center=true);

		// Top layer
		color([0.5,0.8,0.5])  translate(v=[0, 0, 27]) cube(size = [68,53,2], center=true);
		color([0.5,0.5,0.5])  translate(v=[20, 20, 29]) cube(size = [10,5,3], center=true);
		color([1,1,1]) translate(v=[30, 10, 23])   cube(size = [10,8,8], center=true);
		color([1,1,1]) translate(v=[-30, -10, 23])   cube(size = [10,18,8], center=true);
	}
}

module mtsmc_circuit()
{
	union()
	{
		//Main circuit
		color([0.5,0.8,0.5]) difference()
		{
			cube(size = [80,35,2], center=true);
			translate(v=[-37.03, 14.41, -2]) cylinder(h=4, r=1.5);
			translate(v=[37.03, -14.41, -2]) cylinder(h=4, r=1.5);
		}

		// MTSMC Microcontrolles
		color([0.5,0.5,0.5]) translate(v=[-8.5, -4, 2.5]) cube(size = [24,24,3], center=true);
		color([0.5,0.5,0.5]) translate(v=[-29, 0, 2.5]) cube(size = [16,16,3], center=true);

		// Bottom circuit
		union()
		{
			color([0.5,0.8,0.5])  translate(v=[0, 0, -8]) cube(size = [80,35,2], center=true);
			color([0.5,0.5,0.5])  translate(v=[-30,-17,-6]) rotate(a = [90,0,0]) cylinder(h=9, r=4);

			color([0.5,0.5,0.5])  translate(v=[-19,-17,-6]) rotate(a = [90,0,0]) cylinder(h=9, r=4);		
			color([0.5,0.5,0.5])  translate(v=[0,23,-6]) rotate(a = [90,0,0]) cylinder(h=6, r=2);		
		}

		// Wires out
		color([1,1,1]) translate(v=[36,0,-4])  cube(size = [10,8,8], center=true);		
	}
}


module camera_circuit()
{
	difference()
	{
		union()
		{
			color([0.5,0.8,0.5]) cube(size = [32,32,1.5], center=true);

			color([0.5,0.5,0.5]) cylinder(h=24, r=7.75);
			color([0.5,0.5,0.5]) translate([0,0,4]) cube(size = [20,20,8], center=true);
		}

		translate(v=[14,14,-2]) cylinder( h=4, r=1);
		translate(v=[14,-14,-2]) cylinder( h=4, r=1);
		translate(v=[-14,-14,-2]) cylinder( h=4, r=1);
		translate(v=[-14,14,-2]) cylinder( h=4, r=1);
	}
}





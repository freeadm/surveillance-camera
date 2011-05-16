include <Components.scad>
include <TopSphere.scad>
include <Holder.scad>
include <BottomSphere.scad>

difference()
{
	union()
	{
		translate( v=[0,0,0.1] ) top_sphere();
//		holder();
//		translate( v=[0,0,-0.1] ) bottom_sphere();


//		translate( v=[0,0,-36] )   main_circuit();
//		translate( v=[-40,0,3] ) rotate(a=[0,0,90]) status_board();
//		translate( v=[0,-37.5,-19.5] ) battery_holder();
//		translate( v=[0,37.5,-19.5] ) battery_holder();
//		translate( v=[0,0,17] ) rotate(a=[0,0,90]) mtsmc_circuit();
		translate( v=[40,0,20] ) rotate(a=[90,0,90]) camera_circuit();

	}

//	translate( v=[-100,0,-100] ) cube([200,200,200]);
} 
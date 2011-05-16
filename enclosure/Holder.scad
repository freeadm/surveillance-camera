leg_offset=33;

module holder()
{
	union() 
	{
		// Base
		difference()
		{
			translate( v=[0,0,-7] ) cylinder(h=14, r=58.5);
			translate( v=[0,0,-9] ) cylinder(h=18, r=56);

			translate(v=[45,14,6]) rotate(a=[0,90,0]) cylinder(h=20, r=2.5);
			translate(v=[56,14,8.5]) cube(size=[8,5,5], center=true);
			translate(v=[45,-14,6]) rotate(a=[0,90,0]) cylinder(h=20, r=2.5);
			translate(v=[56,-14,8.5]) cube(size=[8,5,5], center=true);
		}

		// Bottom lid
		difference()
		{
			translate(v=[0,0,0])  cylinder(h=3, r=58.5);				
			translate(v=[0,0, -1])  cylinder(h=5, r=25); // Wire hole
			translate( v=[-40,0,3.5] ) cube(size=[22, 26, 4], center=true);
			translate( v=[-40,0,0] ) cube(size=[26, 17, 4], center=true);

			// Holes for top poles
			union()
			{
				for ( x = [ [leg_offset, -leg_offset, 0], 
					[-leg_offset, -leg_offset, 0 ], 
					[leg_offset, leg_offset, 0], 
					[-leg_offset, leg_offset, 0 ] ] ) 
				{
					translate(v=x) 
					{ 
						union() 
						{
							translate(v=[0,0,-1]) cylinder(h=6, r=1.4);
							translate(v=[0,0, -1]) cylinder(h=2.1, r=3);
						}
					}			
				}
			}
		}

		// Top poles holders
		difference()
		{
			union()
			{
				for ( x = [ [leg_offset, -leg_offset, 0], 
						[-leg_offset, -leg_offset, 0 ], 
						[leg_offset, leg_offset, 0], 
						[-leg_offset, leg_offset, 0 ] ] ) 
				{
					translate(v=x)  translate(v=[0,0, 1]) cylinder(h=10, r=5);
				}
			}

			union()
			{
				for ( x = [ [leg_offset, -leg_offset, 0], 
					[-leg_offset, -leg_offset, 0 ], 
					[leg_offset, leg_offset, 0], 
					[-leg_offset, leg_offset, 0 ] ] ) 
				{
					translate(v=x) 
					{ 
						union() 
						{
							translate(v=[0,0, -3]) cylinder(h=18, r=1.4);
							translate(v=[0,0, 2]) cylinder(h=14, r=3.2);
						}
					}			
				}
			}
		}


		// MTSMC Holder
		difference()
		{
			union()
			{
				translate(v=[15, 37, 0]) cylinder(h=8, r=3);
				translate(v=[-15, 37, 0]) cylinder(h=8, r=3);
				translate(v=[-15, -37, 0]) cylinder(h=8, r=3);
				translate(v=[15, -37, 0]) cylinder(h=8, r=3);
			}				
		}
	}		
}
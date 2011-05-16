leg_offset=33;

module top_sphere() 
{
	difference() 
	{
		intersection()
		{
			sphere(r=60, center=true);

			union()
			{	
				// Main body
				difference()
				{	
					sphere(r=60, center=true);
					sphere(r=58, center=true);
					translate(v=[0, 0, -61]) cylinder(h=61, r=60);

					translate(v=[0,0, -1]) cylinder(h=8, r=58.5);

					// Camera hole 
					translate(v=[45,0,20]) rotate(a=[0,90,0]) cylinder(h=15, r=8);
				}


				// Hodler poles
				for ( x = [ [leg_offset, -leg_offset, 0], 
							[-leg_offset, -leg_offset, 0 ], 
							[leg_offset, leg_offset, 0], 
							[-leg_offset, leg_offset, 0 ] ] ) 
				{
					translate(v=x) { 
						difference()
						{
							translate(v=[0,0, 3]) cylinder(h=37, r=3);
							translate(v=[0,0, -1]) cylinder(h=10, r=1.4);						
						}
					}
				}

				// Camera mount
				difference()
				{
					union()
					{
						translate(v=[41,-14,6]) rotate(a=[0,90,0]) cylinder(h=18, r=2);
						translate(v=[41,-14,34]) rotate(a=[0,90,0]) cylinder(h=18, r=2);
						translate(v=[41,14,6]) rotate(a=[0,90,0]) cylinder(h=18, r=2);
						translate(v=[41,14,34]) rotate(a=[0,90,0]) cylinder(h=18, r=2);
					}

					union()
					{
						translate(v=[38,-14,6]) rotate(a=[0,90,0]) cylinder(h=10, r=0.8);
						translate(v=[38,-14,34]) rotate(a=[0,90,0]) cylinder(h=10, r=0.8);
						translate(v=[38,14,6]) rotate(a=[0,90,0]) cylinder(h=10, r=0.8);
						translate(v=[38,14,34]) rotate(a=[0,90,0]) cylinder(h=10, r=0.8);							}
				}

			}
		}
	}
}


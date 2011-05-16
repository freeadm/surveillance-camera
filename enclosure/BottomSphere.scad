module bottom_sphere() 
{
	intersection()
	{
		sphere(r=60, center=true);

		union()
		{
			difference()
			{	
				union()
				{
					// Shell
					sphere(r=60, center=true);
				}
				
				sphere(r=58, center=true);

				translate(v=[0,0, -7]) cylinder(h=8, r=58.5);
				cylinder(h=60, r=60);
				translate( v=[0,-38,-20] ) cube(size = [41,20,17], center=true);
				translate( v=[0,38,-20] ) cube(size = [41,20,17], center=true);
				translate(v=[0, 0, -60]) cylinder(h=20, r=33);
			}

			// Circut holder
			union()
			{
				translate(v=[15, 15, -41]) cylinder(h=4, r=3);
				translate(v=[-15, 15, -41]) cylinder(h=4, r=3);	
				translate(v=[-15, -15, -41]) cylinder(h=4, r=3);
				translate(v=[15, -15, -41]) cylinder(h=4, r=3);
			}				

			// Battery holder base
			difference()
			{
				union()
				{
					translate( v=[0,-38,-38.5] ) cube(size = [45,20,20], center=true);
					translate( v=[0,38,-38.5] ) cube(size = [45,20,20], center=true);
				}
				translate( v=[0,-38,-38.5] ) cube(size = [48,18,18], center=true);
				translate( v=[0,38,-38.5] ) cube(size = [48,18,18], center=true);

				translate(v=[0, 0, -60]) cylinder(h=20, r=33);
			}
			

			// Top extention
			difference() 
			{
				translate(v=[0,0, -9])  cylinder(h=2, r=60); 
				translate(v=[0,0, -11]) cylinder(h=8, r=53);
			}

			
			difference()
			{	
				// Bottom holes
				translate(v=[0, 0, -50]) cylinder(h=10, r=35);
				translate(v=[0, 0, -52]) cylinder(h=10, r=33);
		
				for (x=[-2:2] )
					for (y=[-2:2] )
					{
						translate(v=[8*x, 8*y, -43]) cylinder(h=4, r=1);				
					}					
			}		
		}
	}
}
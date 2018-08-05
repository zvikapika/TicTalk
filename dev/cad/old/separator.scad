difference() {
	union() {
		linear_extrude(height = 60, center = true, convexity = 10) import (file = "cross.dxf");
		translate([0,0,30]) cylinder(3,75, 75);
	}
	translate([0,0,30]) cube([60,55,10], center=true);
}
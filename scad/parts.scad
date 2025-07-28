root2 = sqrt(2);

module torus(r, R, a) {
	rotate_extrude(angle=a, convexity=5)
		translate([R, 0, 0])
			circle(r=r);
}

module cam(r, w, d) {
	w2 = w / 2;
	translate([ 0,  0,  w2])     cylinder(d=d, h=d*2);  // Top shaft
	translate([-d, -d,  w2-d])   cube([r+d*2, d*2, d]); // Top cam arm
	translate([ r,  0, -w2])     cylinder(d=d, h=w);    // Cam shaft
	translate([-d, -d, -w2])     cube([r+d*2, d*2, d]); // Bottom cam arm
	translate([ 0,  0, -w2-d*2]) cylinder(d=d, h=d*3);  // Bottom shaft
}

module rotor(r, R, v) {
	w = 20;
	difference() {
		translate([0, 0, -r/2 - r/10])
			cylinder(r=R, h=r/2);
		torus(r, R, 360);
	}
	for (i = [0:v-1]) {
		rotate(i*360/v - w/2, [0, 0, 1])
			torus(r, R, 20);
	}
}

module arm(l, w, h, j) {
	h2 = h / 2;
	ha = h2 / root2;
	lj = l * j;
	linear_extrude(w)
		polygon([
			[0 , 0],
			[0 , lj],
			[h2, lj+h2],
			[h2, l],
			[h , l],
			[h , lj+h2-ha],
			[h2, lj-ha],
			[h2, 0]
		]);
}

//module arms(

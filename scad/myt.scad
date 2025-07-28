use <parts.scad>;

Vanes = 4; // [2:2:10]
Size = 100;
Arm_Jog = 0.35; // [0:0.01:1]

$fa = $preview ? 10 : 1;
$fs = $preview ? 2.5 : 0.75;

//torus(10, 30, 90);
//arm(40, 10, 10, Arm_Jog);
//rotate($t*360, [0, 0, 1]) cam(r=10, w=20, d=4);
rotor(10, 50, Vanes);

echo(a);

polygon(points=[[0,0],[100,0],[130,50],[30]]);

polyhedron(
  points=[ [10,10,0],[10,-10,0],[-10,-10,0],[-10,10,0],
           [0]  ],
  faces=[ [0,1,4],[1,2,4],[2,3,4],[3,0,4],
              [1,0,3],[2,1,3] ]
 );
 
//file does not exist - we only care about the file ending
import("doesNotExist.aaa");
//file does not exist an thus creates a warning
import("doesNotExist.off");
//unkown module
hello();
//unkown function
b=test();
//radius is ignored as a diameter is defined
circle(r=1,d=4);

rotate(1/0)
rotate([1/0,1/0])
rotate((1/0)/(1/0))
cube();

$vpr="[1,2,3]";
$vpt=[1,2,3,4];
$vpd=[1];

color([-1,0,1,2])
color("notAName")
box();

cylinder($fs=0);
cylinder($fa=0);
cylinder($fn=0);

cylinder($fs=1/0);
cylinder($fa=1/0);
cylinder($fn=1/0);

cylinder($fs="test");
cylinder($fa="test");
cylinder($fn="test");
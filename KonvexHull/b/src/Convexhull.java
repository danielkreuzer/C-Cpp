import java.util.Arrays;

public class Convexhull {

	static class Point {
		public double x;
		public double y;

		public Point(double xin, double yin) {
			x = xin;
			y = yin;
		}

		public String toString() {
			String output = "Point: [" + x + ", " + y + "]";
			return output;
		}
	}
	
	static class PointSet {
		protected int size = 0;
		protected Point[] pSet = null;
		
		public boolean inPointset(Point x) {
			for(int i = 0; i < size; i++) {
				if (pSet[i].x == x.x && pSet[i].y == x.y) {
					return true;
				}
			}
			return false;
		}
		
		public PointSet() {
			pSet = new Point[0];
		}
		// Erzeugt eine leere Punktmenge mit Kapazit�t 0. 
		public PointSet(int capacity) {
			pSet = new Point[capacity];
		}
		// Erzeugt eine leere Punktmenge mit Kapazit�t capacity. 
		public int capacity(){
			int length = pSet.length;
			return length;
		}
		// Gibt die Kapazit�t der Punktmenge zur�ck public int size(); 
		// Gibt die Gr��e der Punktmenge zur�ck. 
		public void changeCapacity(int newCapacity){
			Point[] temp = new Point[newCapacity];
			Arrays.fill(temp, null);
			for(int i = 0; i < size && i < newCapacity; i++) {
				temp[i] = pSet[i];
			}
			pSet = temp;
		}
		// Ver�ndert die Kapazit�t der Punktmenge. newCapacity kann
		// gr��er als, kleiner als oder gleich wie die alte Kapazit�t sein. 
		public void add(Point p){
			if(!inPointset(p)) {
				if(size+1 > pSet.length) {
					changeCapacity(size + 10);
				}
				pSet[size] = p;
				size++;
			}
		}
		// F�gt einen Punkt zur Menge hinzu. Erh�ht bei Bedarf die Kapazit�t. 
		public Point get(int i){
			if(i < size) {
				return pSet[i];
			} else {
				return null;
			}
		}
		// Liefert den i-ten Punkt der Menge. 
		public PointSet unify(PointSet other){
			PointSet temp = new PointSet();
			temp.pSet = pSet;
			int j = other.size();
			for(int i = 0; i < j; i++) {
				temp.add(other.get(i));
			}
			return temp;
		}
		// Vereinigt diese Punktmenge mit der Punktmenge other und gibt eine 
		// neue Punktmenge zur�ck. 
		public String toString() {
			String output = "[";
			for(int i = 0; i < size; i++) {
				output += "(" + pSet[i].x + "," + pSet[i].y + ")";  
				if(i != size-1) {
					output += ",";
				}
			}
			output += "]";
			return output;
		}
		// Erzeugt einen String der Form [(x1,y1), ..., (xn,yn)]. }
		public int size() {
			return size;
		}
		
		public PointSet(ConvexPolygon points) { 
			pSet = new Point[0];
			size = 0;
			for (int i = 0; i < points.size(); i++) {
				this.add(points.get(i));
			}
		}
		
		private static double turningWay(Point a, Point b, Point c)
	    {
	        double val = (b.y - a.y) * (c.x - b.x) -
	                  (b.x - a.x) * (c.y - b.y);
	      
	        if (val == 0) {
	        	return 0;
	        }
	        if (val > 0) {
	        	return 1;
	        } else {
	        	return 2;
	        }
	    }
		// Initialisiert eine Punktmenge mit den Eckpunkten eines 
		// konvexen Polygons.
		public ConvexPolygon convexHull() {
			ConvexPolygon output = new ConvexPolygon();
			
	        if (size < 3) {
	        	return output;
	        }
	      
	        int mostLeft = 0;
	        for (int i = 1; i < size; i++) {
	            if (pSet[i].x < pSet[mostLeft].x) {
	                mostLeft = i;
	            }
	        }
	      
	        //temp = next point in hull to find
	        //mostLeft = starting point
	        //rightToBeat = next Point, to be checked if is in hull
	        int temp = mostLeft, rightToBeat;
	        
	        //Build hull
	        do
	        {
	            output.add(pSet[temp]);
	            rightToBeat = (temp + 1) % size;
	             
	            for (int i = 0; i < size; i++)
	            {
	               if (turningWay(pSet[temp], pSet[i], pSet[rightToBeat]) == 2)
	            	   rightToBeat = i;
	            }
	            temp = rightToBeat;
	      
	        } while (temp != mostLeft);
	        
			return output;
		}
		// Ermittelt die konvexe Menge der Punktmenge.
	}
	
	static class ConvexPolygon extends PointSet {
		public boolean containsPoint(Point p) {
			int i;
			int j;
			boolean result = false;
      		for (i = 0, j = size-1; i < size; j = i++) {
        		if ((pSet[i].y > p.y) != (pSet[j].y > p.y) && (p.x < (pSet[j].x - pSet[i].x) * (p.y - pSet[i].y) / (pSet[j].y-pSet[i].y) + pSet[i].x)) {
          			result = !result;
         		}
      		}
      		return result;
		} // �berpr�ft, ob Punkt p innerhalb des konvexen Polygons liegt. 
		
	}
	
	
    public static void main(String[] args) {
        System.out.println("-----p a.)-----");
        
        System.out.println("* new Pointset");
        PointSet ps1 = new PointSet();
        System.out.println("Capacity: " + ps1.capacity());
        System.out.println("Size: " + ps1.size());
        System.out.println(ps1.toString());
        
        System.out.println("* add Points");
        Point p1 = new Point(1, 2);
        ps1.add(p1);
        ps1.add(new Point(1, 2));
        ps1.add(new Point(3, 6));
        ps1.add(new Point(4, 8));
        ps1.add(new Point(1, 5));
        System.out.println("Capacity: " + ps1.capacity());
        System.out.println("Size: " + ps1.size());
        System.out.println(ps1.toString());
        
        System.out.println("* change capacity to 22");
        ps1.changeCapacity(22);
        System.out.println("Capacity: " + ps1.capacity());
        System.out.println("Size: " + ps1.size());
        System.out.println(ps1.toString());
        
        System.out.println("* Get point at 2");
        System.out.println(ps1.get(2).toString());
        
        System.out.println("-----Test b.)-----");
        System.out.println("* add Points to new PointSet");
        PointSet ps2 = new PointSet();
        ps2.add(new Point(1,1));
        ps2.add(new Point(1,2));
        ps2.add(new Point(2,3));
        ps2.add(new Point(2,2));
        ps2.add(new Point(2,1));
        ps2.add(new Point(3,3));
        ps2.add(new Point(3,2));
        System.out.println("Capacity: " + ps2.capacity());
        System.out.println("Size: " + ps2.size());
        System.out.println(ps2.toString());
        
        System.out.println("* generate Convex hull of PointSet = all Points without 2,2");
        ConvexPolygon cp1 = new ConvexPolygon();
        cp1 = ps2.convexHull();
        System.out.println(cp1.toString());
        
        System.out.println("* add Points to new PointSet");
        PointSet ps3 = new PointSet();
        ps3.add(new Point(1,2));
        ps3.add(new Point(3,2));
        ps3.add(new Point(8,6));
        ps3.add(new Point(9,1));
        ps3.add(new Point(9,12));
        System.out.println("Capacity: " + ps3.capacity());
        System.out.println("Size: " + ps3.size());
        System.out.println(ps3.toString());
        
        System.out.println("* generate Convex hull of PointSet = 1,2 9,1 9,12");
        ConvexPolygon cp2 = new ConvexPolygon();
        cp2 = ps3.convexHull();
        System.out.println(cp2.toString());
        
        System.out.println("* initialise new PointSet with Convex hull");
        PointSet ps6 = new PointSet(cp2);
        System.out.println("Capacity: " + ps6.capacity());
        System.out.println("Size: " + ps6.size());
        System.out.println(ps6.toString());
        
        System.out.println("* add Points to new PointSet");
        PointSet ps4 = new PointSet();
        ps4.add(new Point(1,2));
        ps4.add(new Point(3,2));
        System.out.println("Capacity: " + ps4.capacity());
        System.out.println("Size: " + ps4.size());
        System.out.println(ps4.toString());
        
        System.out.println("* generate Convex hull of PointSet = no hull!");
        ConvexPolygon cp3 = new ConvexPolygon();
        cp3 = ps4.convexHull();
		System.out.println(cp3.toString());
		
		System.out.println("* check if Point 2,2 in ConvexHull = true");
		System.out.println(cp3.containsPoint(new Point(2,2)));

		System.out.println("* check if Point 1,5 in ConvexHull = false");
		System.out.println(cp3.containsPoint(new Point(1,5)));
        
    }
}

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
		private int size = 0;
		private Point[] pSet = null;
		
		private boolean inPointset(Point x) {
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
		
	}
	
	
    public static void main(String[] args) {
        System.out.println("-----Test a.)-----");
        
        System.out.println("* new Pointset");
        PointSet ps1 = new PointSet();
        System.out.println("Capacity: " + ps1.capacity());
        System.out.println("Size: " + ps1.size());
        System.out.println(ps1.toString());
        
        System.out.println("* add Points");
        Point p1 = new Point(1, 2);
        ps1.add(p1);
        ps1.add(new Point(1, 2));
        ps1.add(new Point(1, 5));
        ps1.add(new Point(3, 6));
        ps1.add(new Point(4, 8));
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
        
        
        
    }
}

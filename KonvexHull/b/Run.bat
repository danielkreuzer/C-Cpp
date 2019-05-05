javac -d bin src/*.java
jar cfe Convexhull.jar Convexhull ./bin/*.class
java -classpath bin Convexhull
PAUSE
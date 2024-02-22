SRC_DIR="src"
BIN_DIR="bin"
MAIN_CLASS="MatrixProduct"
OUTPUT="output/22_02/results_java.csv"

# Clean and compile
rm -f $BIN_DIR/*.class

javac -d $BIN_DIR $SRC_DIR/*.java

# Tests
for dim in `seq 600 400 3000`; do
    java -cp $BIN_DIR $MAIN_CLASS 1 $dim $OUTPUT 
done

for dim in `seq 600 400 3000`; do
    java -cp $BIN_DIR $MAIN_CLASS 2 $dim $OUTPUT 
done


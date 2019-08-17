for filename in `find ../src -not \( -path ../src/External -prune \) -name "*.cpp"`; 
do 
	gcov -n -r -o . $filename
done
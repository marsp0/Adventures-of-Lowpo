for filename in `find ../ -name "*.cpp"`; 
do 
	gcov -n -r -o . $filename
done
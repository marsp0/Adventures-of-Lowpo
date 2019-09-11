doxygen -g dconfig
echo "PROJECT_NAME=\"Adventures of Lowpo\"" 	>> dconfig
echo "OUTPUT_DIRECTORY=./docs/" 			>> dconfig
echo "INLINE_SOURCES=YES" 					>> dconfig
echo "SOURCE_BROWSER=YES" 					>> dconfig
echo "RECURSIVE=YES" 					>> dconfig
doxygen dconfig
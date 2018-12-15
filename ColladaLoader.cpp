#include "ColladaLoader.hpp"

// Very simple and slow Collada loader. DO NOT USE


using namespace Collada;

Source::Source(bool isFloatArray, TechniqueCommon techniqueCommon, std::vector<float> floatArray, std::vector<std::string> stringArray, std::string id):
    isFloatArray(isFloatArray), techniqueCommon(techniqueCommon), floatArray(floatArray), stringArray(stringArray), id(id)
{

}

Triangles::Triangles(std::string material, std::vector<Input> inputs) : material(material), inputs(inputs)
{
}

Vertices::Vertices(std::string ID, Input input) : ID(ID), input(input)
{
    
}

Input::Input(std::string semantic, std::string source) : semantic(semantic), source(source)
{
    
}

TechniqueCommon::TechniqueCommon(Accessor accessor) : accessor(accessor)
{

}

Accessor::Accessor(std::string source, int count, int stride, std::vector<Param> params) :
    source(source), count(count), stride(stride), params(params)
{

}

Param::Param(std::string name) : name(name)
{

}

Loader::Loader(std::string& filename)
{
    std::ifstream       colladaFile;
    std::stringstream   colladaFileString;
    colladaFile.open(filename);
    colladaFileString << colladaFile.rdbuf();
    colladaFile.close();
    this->colladaStringFile = colladaFileString.str();

}

std::vector<std::string> Loader::SplitString(std::string stringData)
{
    std::vector<std::string> result;
    size_t prevDelimeter = 0;
    size_t currentDelimeter = stringData.find(" ");
    while (currentDelimeter != std::string::npos)
    {
        result.push_back(stringData.substr(prevDelimeter, currentDelimeter - prevDelimeter));
        prevDelimeter = currentDelimeter;
        currentDelimeter = stringData.find(" ", prevDelimeter+1);
    }
    if (prevDelimeter != currentDelimeter)
    {
        result.push_back(stringData.substr(prevDelimeter, currentDelimeter));
    }
    return result;
}   

GeometryLibrary Loader::ParseGeometry(std::string geometryData)
{
    size_t vertStart = geometryData.find("<vertices");
    size_t vertEnd = geometryData.find("</vertices");
    Vertices verts = this->ParseVertices(geometryData.substr(vertStart,vertEnd-vertStart));

    size_t triStart = geometryData.find("<triangles");
    size_t triEnd = geometryData.find("</triangles");
    Triangles triangles = this->ParseTriangles(geometryData.substr(triStart, triEnd - triStart));

    size_t startIndex = geometryData.find("<source");
    size_t endIndex = geometryData.find("</source");
    std::vector<Source> sources;
    while (endIndex != std::string::npos)
    {
        sources.push_back(this->ParseSource(geometryData.substr(startIndex, endIndex-startIndex)));
        startIndex = endIndex;
        endIndex = geometryData.find("<source", startIndex+1);
    }
}

Source Loader::ParseSource(const std::string& sourceData)
{   
    size_t startIndex = sourceData.find("id=\"")+4;
    size_t endIndex = sourceData.find("\">");
    std::string id = sourceData.substr(startIndex,endIndex-startIndex);

    startIndex = sourceData.find(">");
    endIndex = sourceData.find("<", startIndex);
    std::vector<std::string> result = this->SplitString(sourceData.substr(startIndex, endIndex-startIndex));
    std::vector<float> floatResult;
    bool isFloatArray;
    // found float_array
    if (sourceData.find("float_array") != std::string::npos)
    {
            for (int i = 0; i < result.size(); i++)
            {
                floatResult.push_back(std::stof(result[i]));
            }
            isFloatArray = true;
    }

    TechniqueCommon technique = this->ParseTechnique(sourceData.substr(sourceData.find("technique_common"),sourceData.npos));
    if (isFloatArray)
    {
        return Source(isFloatArray,technique,floatResult,std::vector<std::string>(), id);
    }
    return Source(isFloatArray,technique,std::vector<float>(), result, id);
}

Triangles Loader::ParseTriangles(std::string trianglesData)
{
    std::string searchFor = "material=\"";
    size_t startIndex = trianglesData.find(searchFor) + searchFor.length();
    size_t endIndex = trianglesData.find("\"", startIndex+1);
    std::string material = trianglesData.substr(startIndex, endIndex-startIndex);

    startIndex = trianglesData.find("<input");
    endIndex = trianglesData.find("/>");
    std::vector<Input> inputs;
    while (endIndex != std::string::npos)
    {
        inputs.push_back(this->ParseInput(trianglesData.substr(startIndex, endIndex-startIndex)));
        startIndex = endIndex;
        endIndex = trianglesData.find("<input", startIndex+1);
    }
    return Triangles(material, inputs);
}

TechniqueCommon Loader::ParseTechnique(std::string techniqueData)
{
    return TechniqueCommon(this->ParseAccessor(techniqueData));
}

Accessor Loader::ParseAccessor(std::string accessorData)
{
    size_t startIndex      = accessorData.find("source=\"") + 8;
    size_t endIndex        = accessorData.find("\" count");
    std::string source  = accessorData.substr(startIndex,endIndex-startIndex);
    
    startIndex  = accessorData.find("\" count=") + 8;
    endIndex    = accessorData.find("\" stride");
    int count   = std::stoi(accessorData.substr(startIndex, endIndex-startIndex));
    
    startIndex  = accessorData.find("stride=\"") + 8;
    endIndex    = accessorData.find("\">");
    int stride  = std::stoi(accessorData.substr(startIndex,endIndex-startIndex));

    std::vector<Param> params;
    size_t startSearchIndex = 0;
    for (int i = 0; i < stride; i++)
    {
        startIndex = accessorData.find("<param",startSearchIndex);
        endIndex = accessorData.find("/>");
        params.push_back(this->ParseParam(accessorData.substr(startIndex, endIndex-startIndex)));
        startSearchIndex = endIndex;
    }
}

Param Loader::ParseParam(std::string paramData)
{
    size_t startIndex = paramData.find("name=\"") + 5;
    size_t endIndex = paramData.find("\"",startIndex+1);
    std::string name = paramData.substr(startIndex, endIndex-startIndex);
    return Param(name);
}



Vertices Loader::ParseVertices(std::string verticesData)
{
    size_t start = verticesData.find("id=\"")+4;
    size_t end   = verticesData.find("\"", start+1);
    std::string id = verticesData.substr(start,end-start);
    Input input = Input(this->ParseInput(verticesData));
    return Vertices(id, input);
}

Input Loader::ParseInput(std::string inputData)
{
    std::string position = "POSITION";
    std::string sourceString = "source=";
    std::string semantic = inputData.substr(inputData.find(position),position.length());
    std::string source = inputData.substr(inputData.find(sourceString), source.length());
    return Input(semantic,source);
}
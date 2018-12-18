#include "ColladaLoader.hpp"

// Very simple and slow Collada loader. DO NOT USE


using namespace Collada;

GeometryLibrary::GeometryLibrary(Vertices vertices, Triangles triangles, Source positions, Source normals, Source texCoords) : 
        vertices(vertices), triangles(triangles), positions(positions), normals(normals), texCoords(texCoords)
{

}

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

Loader::Loader(std::string filename)
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
        prevDelimeter = currentDelimeter + 1;
        currentDelimeter = stringData.find(" ", prevDelimeter);
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

    std::string sourceStringOpen = "<source";
    std::string sourceStringClose = "</source>";
    size_t startIndex = geometryData.find(sourceStringOpen);
    size_t endIndex = geometryData.find(sourceStringClose);
    std::vector<Source> sources;
    int positions;
    int normals;
    int texCoords;
    while (endIndex != std::string::npos)
    {
        sources.push_back(this->ParseSource(geometryData.substr(startIndex, endIndex-startIndex)));
        startIndex = endIndex + sourceStringClose.length();
        endIndex = geometryData.find(sourceStringClose, startIndex);
    }
    // set positions
    for (int i = 0; i < sources.size(); i++)
    {
        if(sources[i].id == verts.input.source)
        {
            positions = i;
        }
        else
        {
            for (int j = 0; j < triangles.inputs.size(); j++)
            {
                if (triangles.inputs[j].source == sources[i].id)
                {
                    if (triangles.inputs[j].semantic == "NORMAL")
                        normals = i;
                    else if (triangles.inputs[j].semantic == "TEXCOORD")
                        texCoords = i;
                }
            }
        }
    }
    return GeometryLibrary(verts, triangles, sources[positions], sources[normals], sources[texCoords]);
}

Source Loader::ParseSource(const std::string& sourceData)
{   
    std::string idString = "id=\"";
    size_t startIndex = sourceData.find(idString)+idString.length();
    size_t endIndex = sourceData.find("\"",startIndex);
    std::string id = sourceData.substr(startIndex,endIndex-startIndex);

    startIndex = sourceData.find(">", sourceData.find("<float_array"))+1;
    endIndex = sourceData.find("</float_array>", startIndex);
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
    std::cout << "Parsed source" << std::endl;
    if (isFloatArray)
    {
        return Source(isFloatArray,technique,floatResult,std::vector<std::string>(), id);
    }
    return Source(isFloatArray,technique,std::vector<float>(), result, id);
}

Triangles Loader::ParseTriangles(std::string trianglesData)
{
    std::string materialString = "material=\"";
    size_t startIndex = trianglesData.find(materialString) + materialString.length();
    size_t endIndex = trianglesData.find("\"", startIndex+1);
    std::string material = trianglesData.substr(startIndex, endIndex-startIndex);

    startIndex = trianglesData.find("<input");
    endIndex = trianglesData.find("/>");
    
    std::vector<Input> inputs;
    while (endIndex != std::string::npos)
    {
        inputs.push_back(this->ParseInput(trianglesData.substr(startIndex, endIndex-startIndex)));
        startIndex = endIndex+1;
        endIndex = trianglesData.find("<input", startIndex);
    }
    std::cout << "Parsed triangles" << std::endl;
    return Triangles(material, inputs);
}

TechniqueCommon Loader::ParseTechnique(std::string techniqueData)
{
    std::cout << "Parsed technique" << std::endl;
    return TechniqueCommon(this->ParseAccessor(techniqueData));
}

Accessor Loader::ParseAccessor(std::string accessorData)
{
    std::string sourceString    = "source=\"#";
    size_t startIndex           = accessorData.find(sourceString) + sourceString.length();
    size_t endIndex             = accessorData.find("\"", startIndex);
    std::string source          = accessorData.substr(startIndex,endIndex-startIndex);
    
    std::string countString     = "count=\"";
    startIndex                  = accessorData.find(countString) + countString.length();
    endIndex                    = accessorData.find("\"", startIndex);
    int count                   = std::stoi(accessorData.substr(startIndex, endIndex-startIndex));

    std::string strideString    = "stride=\"";
    startIndex                  = accessorData.find(strideString) + strideString.length();
    endIndex                    = accessorData.find("\"", startIndex);
    int stride                  = std::stoi(accessorData.substr(startIndex,endIndex-startIndex));

    std::vector<Param> params;
    std::string paramStringOpen = "<param";
    std::string paramStringClose = "/>";
    startIndex = accessorData.find(paramStringOpen);
    endIndex = accessorData.find(paramStringClose);
    for (int i = 0; i < stride; i++)
    {
        params.push_back(this->ParseParam(accessorData.substr(startIndex, endIndex-startIndex)));
        startIndex = endIndex + paramStringClose.length();
        endIndex = accessorData.find(paramStringClose, startIndex);
    }
    std::cout << "Parsed accessor" << std::endl;
    return Accessor(source,count,stride,params);
}

Param Loader::ParseParam(std::string paramData)
{
    size_t startIndex = paramData.find("name=\"") + 5;
    size_t endIndex = paramData.find("\"",startIndex+1);
    std::string name = paramData.substr(startIndex, endIndex-startIndex);
    std::cout << "Parsed param" << std::endl;
    return Param(name);
}



Vertices Loader::ParseVertices(std::string verticesData)
{
    std::string idString = "id=\"";
    size_t startIndex    = verticesData.find(idString)+idString.length();
    size_t endIndex      = verticesData.find("\"", startIndex);
    std::string id  = verticesData.substr(startIndex,endIndex - startIndex);
    Input input     = Input(this->ParseInput(verticesData));
    // std::cout << "ID is " << id << std::endl;
    return Vertices(id, input);
}

Input Loader::ParseInput(std::string inputData)
{
    std::string semanticString = "semantic=\"";
    size_t semanticStartIndex = inputData.find(semanticString) + semanticString.length();
    size_t semanticEndIndex = inputData.find("\"", semanticStartIndex);
    std::string semantic = inputData.substr(semanticStartIndex, semanticEndIndex - semanticStartIndex);

    std::string sourceString = "source=\"#";
    size_t sourceStartIndex = inputData.find(sourceString) + sourceString.length();
    size_t sourceEndIndex = inputData.find("\"", sourceStartIndex);
    std::string source = inputData.substr(sourceStartIndex, sourceEndIndex-sourceStartIndex);

    // std::cout << "semantic " <<  semantic << std::endl;
    // std::cout << "source " << source << std::endl;
    // std::cout << "Parsed input" << std::endl;
    return Input(semantic,source);
}
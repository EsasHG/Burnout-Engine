#include "filemanager.h"

#include <QImage>
#include <fstream>
#include <sstream>
#include <QFile>

#include "constants.h"
#include "vertex.h"
#include "Components/landscapecomponent.h"

FileManager::FileManager()
{

}

FileManager::~FileManager()
{
}

std::vector<Vertex>  FileManager::readFile(std::string filename) {
    std::ifstream inn;
    std::string fileWithPath = gsl::assetFilePath + "Meshes/" + filename;

    std::vector<Vertex> vertices;

    inn.open(fileWithPath);

    if (inn.is_open()) {
        int n;
        Vertex vertex;
        inn >> n;
        vertices.reserve(n);
        for (int i=0; i<n; i++) {
            inn >> vertex;
            vertices.push_back(vertex);
        }
        inn.close();
        qDebug() << "TriangleSurface file read: " << QString::fromStdString(filename);

    }
    else
    {
        qDebug() << "Could not open file for reading: " << QString::fromStdString(filename);
    }
    return vertices;
}

void FileManager::writeFile(std::vector<Vertex> inVertices, std::string filename)
{
    std::ofstream ut;
    ut.open(filename.c_str());

    if (ut.is_open())
    {
        auto n = inVertices.size();
        Vertex vertex;
        ut << n << std::endl;
        for (auto it=inVertices.begin(); it != inVertices.end(); it++)
        {
            vertex = *it;
            ut << vertex << std::endl;
        }
        ut.close();
    }
}


std::pair<std::vector<Vertex>, std::vector<unsigned int>> FileManager::readOBJFile(std::string filename)
{
    std::pair<std::vector<Vertex>, std::vector<unsigned int>> vertPair;

    //Open File
    std::string fileWithPath = gsl::assetFilePath + "Meshes/" + filename;
    std::ifstream fileIn;
    //fileIn.open (filename, std::ifstream::in);
    fileIn.open (fileWithPath, std::ifstream::in);
    if(!fileIn)
        qDebug() << "Could not open file for reading: " << QString::fromStdString(filename);

    //One line at a time-variable
    std::string oneLine;
    //One word at a time-variable
    std::string oneWord;

    std::vector<gsl::Vector3D> tempVertecies;
    std::vector<gsl::Vector3D> tempNormals;
    std::vector<gsl::Vector2D> tempUVs;

    //    std::vector<Vertex> mVertices;    //made in VisualObject
    //    std::vector<GLushort> mIndices;   //made in VisualObject

    // Varible for constructing the indices vector
    unsigned int temp_index = 0;

    //Reading one line at a time from file to oneLine
    while(std::getline(fileIn, oneLine))
    {
        //Doing a trick to get one word at a time
        std::stringstream sStream;
        //Pushing line into stream
        sStream << oneLine;
        //Streaming one word out of line
        oneWord = ""; //resetting the value or else the last value might survive!
        sStream >> oneWord;

        if (oneWord == "#")
        {
            //Ignore this line
            //            qDebug() << "Line is comment "  << QString::fromStdString(oneWord);
            continue;
        }
        if (oneWord == "")
        {
            //Ignore this line
            //            qDebug() << "Line is blank ";
            continue;
        }
        if (oneWord == "v")
        {
            //            qDebug() << "Line is vertex "  << QString::fromStdString(oneWord) << " ";
            gsl::Vector3D tempVertex;
            sStream >> oneWord;
            tempVertex.x = std::stof(oneWord);
            sStream >> oneWord;
            tempVertex.y = std::stof(oneWord);
            sStream >> oneWord;
            tempVertex.z = std::stof(oneWord);

            //Vertex made - pushing it into vertex-vector
            tempVertecies.push_back(tempVertex);

            continue;
        }
        if (oneWord == "vt")
        {
            //            qDebug() << "Line is UV-coordinate "  << QString::fromStdString(oneWord) << " ";
            gsl::Vector2D tempUV;
            sStream >> oneWord;
            tempUV.x = std::stof(oneWord);
            sStream >> oneWord;
            tempUV.y = std::stof(oneWord);

            //UV made - pushing it into UV-vector
            tempUVs.push_back(tempUV);

            continue;
        }
        if (oneWord == "vn")
        {
            //            qDebug() << "Line is normal "  << QString::fromStdString(oneWord) << " ";
            gsl::Vector3D tempNormal;
            sStream >> oneWord;
            tempNormal.x = std::stof(oneWord);
            sStream >> oneWord;
            tempNormal.y = std::stof(oneWord);
            sStream >> oneWord;
            tempNormal.z = std::stof(oneWord);

            //Vertex made - pushing it into vertex- vector
            tempNormals.push_back(tempNormal);
            continue;
        }
        if (oneWord == "f")
        {
            //            qDebug() << "Line is a face "  << QString::fromStdString(oneWord) << " ";
            //int slash; //used to get the / from the v/t/n - format
            int index, normal, uv;
            for(int i = 0; i < 3; i++)
            {
                sStream >> oneWord;     //one word read
                std::stringstream tempWord(oneWord);    //to use getline on this one word
                std::string segment;    //the numbers in the f-line
                std::vector<std::string> segmentArray;  //temp array of the numbers
                while(std::getline(tempWord, segment, '/')) //splitting word in segments
                {
                    segmentArray.push_back(segment);
                }
                index = std::stoi(segmentArray[0]);     //first is vertex
                if (segmentArray[1] != "")              //second is uv
                    uv = std::stoi(segmentArray[1]);
                else
                {
                    //qDebug() << "No uvs in mesh";       //uv not present
                    uv = 0;                             //this will become -1 in a couple of lines
                }
                normal = std::stoi(segmentArray[2]);    //third is normal

                //Fixing the indexes
                //because obj f-lines starts with 1, not 0
                --index;
                --uv;
                --normal;

                if (uv > -1)    //uv present!
                {
                    Vertex tempVert(tempVertecies[index], tempNormals[normal], tempUVs[uv]);
                    vertPair.first.push_back(tempVert);
                }
                else            //no uv in mesh data, use 0, 0 as uv
                {
                    Vertex tempVert(tempVertecies[index], tempNormals[normal], gsl::Vector2D(0.0f, 0.0f));
                    vertPair.first.push_back(tempVert);
                }
                //vertPair.second.push_back(temp_index++);
            }

            //For some reason the winding order is backwards so fixing this by swapping the last two indices
            //Update: this was because the matrix library was wrong - now it is corrected so this is no longer needed.
//            unsigned int back = mIndices.size() - 1;
//            std::swap(mIndices.at(back), mIndices.at(back-1));
            continue;
        }
    }

    //beeing a nice boy and closing the file after use
    fileIn.close();
    qDebug() << "Obj file read: " << QString::fromStdString(filename);

    return vertPair;

}

std::vector<Vertex> FileManager::readHeightMap(QString filename, LandscapeComponent *lComp)
{
    std::vector<Vertex> vertices;
    vertices.clear();
    QImage image(filename);
    if(!image.isNull())
    {
        unsigned width = image.width();
        unsigned height = image.height();
        vertices.reserve(width*height);
        gsl::Vector2D uv;
        for (int x=0; x<width; x++)
        {
             uv.x = static_cast<float>(x)/ width;
            for (int z=0; z<height; z++)
            {
                uv.y =  1.f- (static_cast<float>(z) / height);
                float height = static_cast<float>(image.pixelColor(x,z).green());
                height /= 255.f;

                height = (height)* (lComp->ymax-lComp->ymin) +lComp->ymin;
                float xVert = x*lComp->h+lComp->xmin;
                float zVert = z*lComp->h+lComp->zmin;
                vertices.push_back(Vertex({xVert,height, zVert},{0,1,0},uv));
            }
        }
    }
    else
    {
        qDebug() << "Image could not be loaded!";
    }
    return vertices;
}

void FileManager::writeHeightMap(QString filename, const LandscapeComponent* lComp, std::vector<Vertex> vertices)
{

    QImage img(lComp->mWidth,lComp->mHeight, QImage::Format_RGB32);
    QRgb value = qRgb(0,0,0);
    img.fill(value);

    for (unsigned x=0; x<lComp->mWidth; x++)
    {       
        for (unsigned z=0; z<lComp->mHeight; z++)
        {       
            unsigned index = (x*lComp->mWidth)+z;
            float height = (vertices[index].mXYZ.y-lComp->ymin)/(lComp->ymax-lComp->ymin);
            int uHeight = static_cast<int>(height*255.f);
            value = qRgb(uHeight,uHeight, uHeight);
            img.setPixelColor(x,z,value);
        }
    }

    if(!img.save(filename))
    {
        qDebug() << "Image could not be saved!";
    }
}
QString FileManager::readScriptFile(QString filename)
{
    //Make a QFile for it
    QFile scriptFile(filename);

    //Try to open file and give error if something is wrong
    if (!scriptFile.open(QIODevice::ReadOnly))
        qDebug() << "Error - NO FILE HERE: " << filename;

//    //reads the file
    QTextStream stream(&scriptFile);
    QString contents = stream.readAll();
    //close the file, because we don't need it anymore
    scriptFile.close();
    return contents;

}

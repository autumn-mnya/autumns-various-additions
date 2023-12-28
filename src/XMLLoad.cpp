#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <variant>
#include <functional>
#include "tinyxml2.h"

#include "XMLLoad.h"

#include "ModSettings.h"
#include "cave_story.h"
#include "File.h"
#include "main.h"
#include "TextScriptVAR.h"

using namespace tinyxml2;
using namespace std;

enum NumberType
{
    Num_Type_Normal,
    Num_Type_Variable,
    Num_Type_Assembly,
    // Add more as needed
};

enum XMLBool
{
    XMLFalse,
    XMLTrue,
};

struct AnimationFrame
{
    RECT rect;
};

struct BitmapInfo
{
    int x;
    int y;
    RECT rect;
    SurfaceID surf_no;
};

struct NumberInfo
{
    int x;
    int y;
    NumberType numberType;
    XMLBool zeros;
    int varNum;  // Add a variable to store the variable number
    string requiredFlags;  // Add a variable to store the required flags
    unsigned int address;  // Add address for Assembly type
};

LoadedXMLData loadedData; // Global variable to store loaded XML data

bool CheckRequiredFlags(const tinyxml2::XMLElement* element) {
    const char* flagsStr = element->Attribute("RequiredFlags");
    if (!flagsStr) {
        // If RequiredFlags is not present, consider it satisfied
        return true;
    }

    // Convert the comma-separated list of flags to integers
    std::vector<long> requiredFlags;
    const char* flagPtr = flagsStr;
    char* end;

    while (*flagPtr != '\0') {
        long flag = std::strtol(flagPtr, &end, 10);
        requiredFlags.push_back(flag);

        if (*end == ',') {
            flagPtr = end + 1; // Move past the comma
        }
        else {
            break; // No more commas, exit the loop
        }
    }

    // Check if all the required flags are set
    for (long requiredFlag : requiredFlags) {
        if (!GetNPCFlag(requiredFlag)) {
            return false; // If any flag is not set, return false
        }
    }

    return true; // All required flags are set
}

// Define the function signature for your number type functions
using NumberTypeFunction = function<int()>;

// Map to associate string names with function pointers
unordered_map<string, NumberTypeFunction> numberTypeMap = {
    {"PlayerLife", []() { return static_cast<int>(gMC.life); }},
    {"PlayerDirection", []() { return gMC.direct; }},
    // Add more as needed
};

// Function to convert the string to an int value
int GetNumberTypeValue(const char* typeStr) {
    auto it = numberTypeMap.find(typeStr);
    if (it != numberTypeMap.end()) {
        int value = it->second();
        // printf("Mapping '%s' to value %d\n", typeStr, value);
        return value;
    }
    // printf("Error: Unknown 'numberType' attribute value: %s\n", typeStr);
    return 0; // Handle error appropriately
}

tinyxml2::XMLDocument xmlDoc;
tinyxml2::XMLElement* root;

void LoadAndExecuteFunctions(const char* fileName)
{
    if (!loadedData.isLoaded)
    {
        if (xmlDoc.LoadFile(fileName) != tinyxml2::XML_SUCCESS)
        {
            printf("Error loading XML file: %s\n", fileName);
            return;
        }

        root = xmlDoc.FirstChildElement("Data");
        if (!root)
        {
            printf("No root element found in the XML.\n");
            return;
        }

        printf("Root element name: %s\n", root->Name());

        // Process SoundElements and populate loadedSoundIDs map if needed

        loadedData.isLoaded = true;
    }

    // Process Bitmap elements

    tinyxml2::XMLElement* bitmapRoot = root->FirstChildElement("Bitmap");
    printf("bitmapRoot: %p\n", bitmapRoot);
    if (bitmapRoot)
    {
        for (tinyxml2::XMLElement* imageElement = bitmapRoot->FirstChildElement("Image"); imageElement; imageElement = imageElement->NextSiblingElement("Image"))
        {
            if (!CheckRequiredFlags(imageElement)) {
                // Skip this image element if required flags are not satisfied
                continue;
            }

            BitmapInfo bitmapInfo;

            if (imageElement->QueryIntAttribute("x", &bitmapInfo.x) != XML_SUCCESS)
            {
                printf("Error reading 'x' attribute from Bitmap element.\n");
                continue;
            }

            if (imageElement->QueryIntAttribute("y", &bitmapInfo.y) != XML_SUCCESS)
            {
                printf("Error reading 'y' attribute from Bitmap element.\n");
                continue;
            }

            XMLElement* rectElement = imageElement->FirstChildElement("Rect");
            if (rectElement)
            {
                if (sscanf(rectElement->Attribute("rect"), "%d,%d,%d,%d", &bitmapInfo.rect.left, &bitmapInfo.rect.top, &bitmapInfo.rect.right, &bitmapInfo.rect.bottom) != 4)
                {
                    printf("Error reading 'rect' attribute from Bitmap element.\n");
                    continue;
                }
            }

            // Loading SurfaceID from XML
            int surfaceIDValue = 0;
            if (imageElement->QueryIntAttribute("surface", &surfaceIDValue) != XML_SUCCESS)
            {
                printf("Error reading 'surface' attribute from Bitmap element.\n");
                continue;
            }

            // Convert int to SurfaceID
            SurfaceID surfaceID = static_cast<SurfaceID>(surfaceIDValue);

            PutBitmap3(&grcGame, bitmapInfo.x, bitmapInfo.y, &bitmapInfo.rect, surfaceID);
        }
    }
    else
    {
        printf("Error: No 'Bitmap' element found in the XML.\n");
    }

    // Process Number elements
    tinyxml2::XMLElement* numberRoot = root->FirstChildElement("Number");
    // printf("numberRoot: %p\n", numberRoot);
    if (numberRoot)
    {
        for (XMLElement* numElement = numberRoot->FirstChildElement("Num"); numElement; numElement = numElement->NextSiblingElement("Num"))
        {
            if (!CheckRequiredFlags(numElement)) {
                // Skip this image element if required flags are not satisfied
                continue;
            }

            int value = 0;
            NumberInfo numberInfo;

            if (numElement->QueryIntAttribute("x", &numberInfo.x) != XML_SUCCESS)
            {
                printf("Error reading 'x' attribute from Number element.\n");
                continue;
            }

            if (numElement->QueryIntAttribute("y", &numberInfo.y) != XML_SUCCESS)
            {
                printf("Error reading 'y' attribute from Number element.\n");
                continue;
            }

            const char* typeStr = numElement->Attribute("numberType");
            if (typeStr)
            {
                if (strcmp(typeStr, "Variable") == 0)
                {
                    // For Variable type, read the VarNum attribute
                    if (numElement->QueryIntAttribute("VarNum", &numberInfo.varNum) != XML_SUCCESS)
                    {
                        printf("Error reading 'VarNum' attribute from Variable element.\n");
                        continue;
                    }

                    // Set the numberType accordingly
                    numberInfo.numberType = Num_Type_Variable;
                }
                else if (strcmp(typeStr, "Assembly") == 0)
                {
                    // For Assembly type, read the Address attribute
                    if (numElement->QueryUnsignedAttribute("Address", &numberInfo.address) != XML_SUCCESS)
                    {
                        printf("Error reading 'Address' attribute from Assembly element.\n");
                        continue;
                    }

                    // Set the numberType accordingly
                    numberInfo.numberType = Num_Type_Assembly;
                }
                else
                {
                    // For other types, use the existing approach
                    value = GetNumberTypeValue(typeStr);
                    numberInfo.numberType = Num_Type_Normal;
                }
            }
            else
            {
                printf("Missing 'numberType' attribute in Number element.\n");
                continue;
            }

            if (numElement->QueryBoolAttribute("zeros", reinterpret_cast<bool*>(&numberInfo.zeros)) != XML_SUCCESS)
            {
                printf("Error reading 'zeros' attribute from Number element.\n");
                continue;
            }

            // Use numberInfo.varNum if numberInfo.numberType is Variable
            if (numberInfo.numberType == Num_Type_Variable)
            {
                // Check if varNum is within the valid range
                if (numberInfo.varNum >= 0 && numberInfo.varNum < TSC_VAR_MAX)
                {
                    value = varData[numberInfo.varNum];
                }
                else
                {
                    printf("Error: Invalid 'VarNum' value: %d\n", numberInfo.varNum);
                    continue;
                }
            }

            if (numberInfo.numberType == Num_Type_Assembly)
            {
               value = (*(int*)numberInfo.address);
            }

            PutNumber4(numberInfo.x, numberInfo.y, value, numberInfo.zeros);
        }
    }
    /*
    else
    {
        printf("Error: No 'Number' element found in the XML.\n");
    }
    */

}
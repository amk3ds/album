#include "Photo.hpp"
#include "Collection.hpp"

int main(int argc, char **argv) 
{
    try 
    {
        Collection collection;
        collection.AddPhoto("./xmas.ppm");
        collection.AddPhoto("./xmas.ppm");
        collection.AddPhoto("./winnie.ppm");
        collection.GetPhoto(0).save("./out.ppm");
    }
    catch (const std::exception &e) 
    {
        fprintf(stderr, "Error: %s\n", e.what());
    }
 
    return 0;
}